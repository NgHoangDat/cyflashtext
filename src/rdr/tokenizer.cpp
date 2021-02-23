#include "../utils/utils.h"

#include "vocab.h"
#include "regex.h"
#include "tokenizer.h"

using namespace tokenizer;


namespace tokenizer::validator {
    Set<Code> get_codes(Vec<Vec<String>> charset) {
        Set<Code> codes;

        for (Vec<String> chars: charset) {
            for (String ch: chars) {
                for (Code code: chop(ch)) {
                    codes.insert(code);
                }
            }
        }

        return codes;
    }

    const Set<Code> PUNCTUATIONS = get_codes({ vocab::PUNCTUATIONS });

    const Set<Code> ALPHABET = get_codes({ vocab::DIGITS, vocab::LOWER_CHARS, vocab::UPPER_CHARS });

    const Set<Code> LETTERS = get_codes({ vocab::LOWER_CHARS, vocab::UPPER_CHARS });

    const Set<Code> DIGITS = get_codes({ vocab::DIGITS });

    const Set<Code> UPPER_CHARS = get_codes({ vocab::UPPER_CHARS });

    const Set<Code> LOWER_CHARS = get_codes({ vocab::LOWER_CHARS });

    const Code STOP_CODE = chop(vocab::STOP).front();

    bool is_brace(const String  &str) {
        return std::find(vocab::BRACES.begin(), vocab::BRACES.end(), str) < vocab::BRACES.end();
    }

    bool has_puntuation(Vec<Code> codes) {
        for (Code code: codes) {
            if (PUNCTUATIONS.count(code)) {
                return true;
            }
        }
        return false;
    }

    bool is_alphabet(Code code) {
        return ALPHABET.count(code) > 0;
    }

    bool is_uppercase(Code code) {
        return UPPER_CHARS.count(code) > 0;
    }

    bool is_lowercase(Code code) {
        return LOWER_CHARS.count(code) > 0;
    }

    bool is_letter(Code code) {
        return LETTERS.count(code) > 0;
    }

    bool is_digit(Code code) {
        return DIGITS.count(code) > 0;
    }
}

    
template <typename T>
void tokenizer::concatinate(Vec<T> target, Vec<Vec<T>> vectors) {
    for (Vec<String> vec: vectors) {
        target.insert(target.end(), vec.begin(), vec.end());
    }
}

Vec<String> tokenizer::tokenize(const String &str) {
    String text = normalize(str);

    if (text.empty()) {
        return {};
    }

    Vec<String> temp_tokens = split(text, vocab::SPACE);
    Vec<String> tokens;


    for (String token: temp_tokens) {
        Vec<Code> codes = chop(token);
        
        if (codes.size() == 1 || validator::has_puntuation(codes)) {
            tokens.push_back(token);
            continue;
        }

        if (ends_with(token, vocab::COMMA)) {
            concatinate(tokens, { tokenize(token.substr(0, token.size() - 1)) });
            continue;
        }

        if (vocab::tokenizer::abbreviation.count(token)) {
            tokens.push_back(token);
            continue;
        }

        const Code codepoint = codes.at(codes.size() - 2);

        if (ends_with(token, vocab::STOP) && validator::is_alphabet(codepoint)) {
            if ((codes.size() == 2 && validator::is_uppercase(codepoint)) || std::regex_search(token, regex::tokenizer::SHORT_NAME)) {
                tokens.push_back(token);
                continue;
            }
            concatinate(tokens, { tokenize(token.substr(0, token.size() - 1)) });
            tokens.push_back(vocab::STOP);
            continue;
        }

        if (vocab::tokenizer::exceptions.find(token) != vocab::tokenizer::exceptions.end()) {
            tokens.push_back(token);
            continue;
        }

        auto check = false;
        for (Set<String> sp_case: { vocab::tokenizer::abbreviation, vocab::tokenizer::exceptions }) {
            for (String elem: sp_case) {
                auto idx = token.find(elem);

                if (idx != String::npos) {
                    check = true;
                    recursive(tokens, token, idx, idx + elem.length());
                    break;
                }
            }

            if (check) break;
        }
        
        if (check) continue;

        bool matching = false;

        for (auto item: regex::tokenizer::REGEXES) {
            if (std::regex_match(token, item.second)) {
                tokens.push_back(token);
                matching = true;
                break;
            }
        }

        if (matching) continue;

        for (auto item: regex::tokenizer::REGEXES) {
            std::smatch matches;
            if (std::regex_search(token, matches, item.second)) {
                if (item.first.compare("url") == 0) {
                    Vec<String> elements = split(token, ".");
                    auto has_url = true;

                    for (String elem: elements) {
                        if (elem.size() == 1 && isupper(elem.at(0))) {
                            has_url = false;
                            break;
                        } 
                    }

                    if (has_url) {
                        for (auto match: matches) {
                            recursive(tokens, token, match.first - token.begin(), match.second - token.begin());
                        }
                    } else {
                        continue;
                    }
                } else if (item.first.compare("month") == 0) {
                    auto has_letter = false;
                    size_t start = matches[0].first - token.begin();
                    size_t end = matches[0].second - token.begin();
                    size_t idx = 0;

                    for (auto code: chop(token)) {
                        if (idx == start) {
                            break;
                        }
                        idx++;

                        if (validator::is_letter(code)) {
                            recursive(tokens, token, start, end);
                            has_letter = true;
                            break;
                        }
                    }

                    if (!has_letter) {
                        tokens.push_back(token);
                    }
                } else {
                    size_t start = matches[0].first - token.begin();
                    size_t end = matches[0].second - token.begin();
                    recursive(tokens, token, start, end);
                }

                matching = true;
                break;
            }
        }

        if (!matching) {
            tokens.push_back(token);
        }
    };

    return tokens;
}

void tokenizer::recursive(Vec<String> tokens, const String &token, int begin, int end) {
    if (begin > 0) {
        concatinate(tokens, { tokenize(token.substr(0, begin)) });
        concatinate(tokens, { tokenize(token.substr(begin, end)) });
    }

    if (end < token.size()) {
        concatinate(tokens, { tokenize(token.substr(end, token.size())) });
    }
}

String tokenizer::join_sentence(Vec<String> tokens) {
    String sentence;

    for (size_t i = 0; i < tokens.size(); i++) {
        String token = tokens.at(i);

        if (!token.empty() && token.compare(vocab::SPACE) > 0) {
            sentence.append(token);

            if (i < tokens.size() - 1) {
                sentence.append(vocab::SPACE);
            }
        }
    }

    return normalize(sentence);
}

Vec<String> tokenizer::join_sentences(Vec<String> tokens) {
    Vec<String> sentences, sentence;

    for (size_t i = 0; i < tokens.size(); i++) {
        String token = tokens.at(i);

        String next_token, prev_token;
        Vec<Code> next_codes, prev_codes;

        if (i < tokens.size() - 1) {
            next_token = tokens.at(i + 1);
            next_codes = chop(next_token.begin(), next_token.end());
        }

        if (i > 0) {
            prev_token = tokens.at(i - 1);
            prev_codes = chop(prev_token.begin(), prev_token.end());
        }

        sentence.push_back(token);

        if (!next_token.empty() && i < tokens.size() - 2 && token.compare(vocab::COLON) == 0) {
            if (validator::is_digit(next_codes.front()) && tokens.at(i + 2).compare(vocab::STOP) == 0 || tokens.at(i + 2).compare(vocab::COMMA)) {
                sentences.push_back(join_sentence(sentence));
                sentence.clear();
                continue;
            }
        }

        if (std::regex_match(token, regex::tokenizer::EOS_PUNCTUATION)) {
            if (!next_token.empty()) {
                if (next_token.compare("\"") == 0 || next_token.compare("''") == 0) {
                    auto count = 0;

                    for (String sent_token: sentence) {
                        if (sent_token.compare("\"") == 0 || sent_token.compare("''") == 0) {
                            count++;
                        }
                    }

                    if (count % 2 == 1) {
                        continue;
                    }
                }

                if (validator::is_brace(next_token) 
                    || next_token.empty()
                    || next_token.compare(vocab::COMMA) 
                    || validator::is_lowercase(next_codes.front())
                    || validator::is_digit(next_codes.front())
                ) {
                    continue;
                }
            }

            if (!prev_token.empty() || sentence.size() == 2 && token.compare(vocab::STOP)) {
                if (validator::is_digit(prev_codes.front()) || validator::is_lowercase(prev_codes.front())) {
                    continue;
                }

                if (validator::is_uppercase(prev_codes.front()) && prev_codes.size() == 1) {
                    continue;
                }
            }

            sentences.push_back(join_sentence(sentence));
            sentence.clear();
        }
    }

    return sentences;
}