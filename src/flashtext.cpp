#include <array>
#include <locale>
#include <regex>

#include "../lib/utf8.h"
#include "flashtext.h"

using namespace flashtext;

const std::regex SPACE("\\s+");
const std::array<String, 195> NON_BOUNDARY_CHARS = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", 
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "_", 
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", 
    "Ă", "ă", "À", "Á", "Â", "Ã", "à", "á", "â", "Ạ", "ạ", "Ả", "ả", "Ấ", "ấ", "Ầ", "ầ", "Ẩ", "ẩ", "Ẫ", "ẫ", "Ậ", "ậ", "Ắ", "ắ", "Ằ", "ằ", "Ẳ", "ẳ", "Ẵ", "ẵ", "Ặ", "ặ", "ã", 
    "Ò", "Ó", "Ô", "Õ", "ò", "ó", "ô", "õ", "Ơ", "ơ", "Ọ", "ọ", "Ỏ", "ỏ", "Ố", "ố", "Ồ", "ồ", "Ổ", "ổ", "Ỗ", "ỗ", "Ộ", "ộ", "Ớ", "ớ", "Ờ", "ờ", "Ở", "ở", "Ỡ", "ỡ", "Ợ", "ợ",
    "È", "É", "Ê", "Ẹ", "ẹ", "Ẻ", "ẻ", "Ẽ", "ẽ", "Ế", "ế", "Ề", "ề", "Ể", "ể", "Ễ", "ễ", "Ệ", "ệ", "è", "é", "ê", 
    "Ù", "Ú", "ù", "ú", "Ũ", "ũ", "Ư", "ư", "Ụ", "ụ", "Ủ", "ủ", "Ứ", "ứ", "Ừ", "ừ", "Ử", "ử", "Ữ", "ữ", "Ự", "ự", 
    "Ì", "Í", "ì", "í", "Ĩ", "ĩ", "Ỉ", "ỉ", "Ị", "ị", 
    "Ý", "ý", "Ỳ", "ỳ", "Ỵ", "ỵ", "Ỷ", "ỷ", "Ỹ", "ỹ"
};

const Set<Code> NON_BOUNDARY_CODE = []() {
    Set<Code> non_boundary_code;
    
    for(auto val: NON_BOUNDARY_CHARS) {
        auto it = val.begin();
        Code code = utf8::next(it, val.end());
        non_boundary_code.insert(code);
    }

    return non_boundary_code;
}();

template <typename Iter>
Vec<Code> flashtext::split_unicode(Iter begin, Iter end) {
    Iter curr = begin;

    Vec<Code> chars;
    while (curr < end) {
        Code code = utf8::next(curr, end);

        if (code != 0) {
            chars.push_back(code);
        }
    }

    return chars;
}

template <typename Iter>
Vec<Code> flashtext::lower(Iter begin, Iter end) {
    using namespace std;
    setlocale(LC_ALL, "");
    Vec<Code> codes = split_unicode(begin, end);
    Vec<Code> chars;  

    for (auto code: codes) {
        chars.push_back(towlower(code));
    }

    return chars;
}

String flashtext::substring(const String &str, int start, int end) {
    auto curr_ptr = str.begin();
    auto start_ptr = str.begin();
    auto end_ptr = str.end();
    
    int curr_idx = 0;

    while (curr_idx < end) {
        if (curr_ptr == str.end()) {
            break;
        }
        
        char ch = utf8::next(curr_ptr, str.end());

        if (ch != 0) {
            curr_idx++;
            if (curr_idx == start) {
                start_ptr = curr_ptr;
            }

            if (curr_idx == end) {
                end_ptr = curr_ptr;
            }

        }
    }

    return String(start_ptr, end_ptr);
}

String flashtext::trim(const String &str) {
    const auto begin = str.find_first_not_of(" ");
    if (begin == String::npos) {
        return "";
    }

    const auto end = str.find_last_not_of(" ");
    const auto range = end - begin + 1;
    return str.substr(begin, range);
}

String flashtext::normalize(const String &str) {
    String norm_str = std::regex_replace(str, SPACE, " ");
    return trim(norm_str);
}

String flashtext::hash(Vec<Code> codes) {
    String val = "";

    for (auto code: codes) {
        val.append(":");
        val.append(std::to_string(code));
    }

    return val;
}

struct flashtext::Node {
    Map<Code, std::shared_ptr<Node>> next;
    bool flag = false;

    template <typename Iter>
    void put(Iter begin, Iter end) {
        if (begin == end) {
            flag = true;
            return;
        }
        
        Code code = *begin;
        if (!next.count(code)) {
            next[code] = std::make_shared<Node>(Node());
        }
        
        next[code]->put(begin + 1, end);
    }

    template <typename Iter>
    void pop(Iter begin, Iter end) {
        if (begin == end) {
            flag = false;
            return;
        }

        Code code = *begin;

        if (!next.count(code)) {
            return;
        }

        next[code]->pop(begin + 1, end);
        
        if (next[code]->next.size() < 1 && !next[code]->flag) {
            next.erase(code);
        }
    }
};

struct flashtext::Metadata {
    String name;

    Metadata();
    Metadata(String name) {
        this->name = name;
    }
};


struct flashtext::Match {
    int start;
    int end;
    String text;
    Metadata *data;

    Match(int start, int end) {
        this->start = start;
        this->end = end;
    }

    Match(int start, int end, String text, Metadata *data) {
        this->start = start;
        this->end = end;
        this->text = text;
        this->data = data;
    }
};

struct flashtext::KeywordProcessor {
    std::shared_ptr<Node> trie;
    Map<String, std::shared_ptr<Metadata>> metadata;
    bool case_sensitive;

    KeywordProcessor(bool case_sensitive) {
        this->case_sensitive = case_sensitive;
        this->metadata.clear();
        this->trie = std::make_shared<Node>(Node());
    }
    ~KeywordProcessor() {
        this->metadata.clear();
    }

    void put(const String &text, std::shared_ptr<Metadata> meta) {
        if (!metadata.count(text)) {
            String norm_text = normalize(text);

            Vec<Code> codes;
            if (case_sensitive) codes = split_unicode(norm_text.begin(), norm_text.end());
            else codes = lower(norm_text.begin(), norm_text.end());

            trie.get()->put(codes.begin(), codes.end());
            metadata[hash(codes)] = meta;
        }
    }

    Vec<Match> extract(const String &text) {
        String norm_text = normalize(text);
        Vec<Code> codes;
        
        if (case_sensitive) codes = split_unicode(norm_text.begin(), norm_text.end());
        else codes = lower(norm_text.begin(), norm_text.end());

        if (codes.size() == 0) {
            return {};
        }

        auto curr = trie.get();
        auto start = 0;
        auto end = 0;
        auto reset = false;
        auto size = codes.size();
        unsigned int idx = 0;

        Vec<Match> matches;

        while (idx < size) {
            auto code = codes.at(idx);

            if (!NON_BOUNDARY_CODE.count(code)) {
                if (curr->flag || curr->next.count(code)) {
                    auto seq_start = 0;
                    auto seq_end = 0;
                    auto is_longest_seq_found = false;

                    if (curr->flag) {
                        end = idx;
                        seq_start = start;
                        seq_end = end;
                    }

                    if (curr->next.count(code)) {
                        auto child = curr->next[code].get();
                        auto idy = idx + 1;

                        while (idy < size) {
                            auto inner = codes.at(idy);

                            if (!NON_BOUNDARY_CODE.count(inner)) {
                                if (child->flag) {
                                    end = idy;
                                    is_longest_seq_found = true;
                                    seq_start = start;
                                    seq_end = end;
                                }
                            }

                            if (child->next.count(inner)) {
                                child = child->next[inner].get();
                            } else {
                                break;
                            }

                            idy++;
                        }

                        if (idy >= size) {
                            if (child->flag) {
                                end = idy + 1;
                                is_longest_seq_found = true;
                                seq_start = start;
                                seq_end = end;
                            }
                        }

                        if (is_longest_seq_found) {
                            idx = end;
                        }
                    }

                    if (seq_end - seq_start > 0) {
                        matches.push_back(Match(seq_start, seq_end));
                    }
                }

                curr = trie.get();
                reset = true;
            } else {
                if (curr->next.count(code)) {
                    curr = curr->next[code].get();
                } else {
                    curr = trie.get();
                    reset = true;

                    auto idy = idx + 1;

                    while (idy < size) {
                        auto inner = codes.at(idy);

                        if (!NON_BOUNDARY_CODE.count(inner)) {
                            break;
                        }

                        idy++;
                    }

                    idx = idy;
                }
            }

            if (idx + 1 >= size) {
                if (curr->flag) {
                    matches.push_back(Match(start, end));
                }
            }

            idx++;

            if (reset) {
                reset = false;
                start = idx;
            }
        }

        return update_matches(norm_text, matches);
    }

    Vec<Match> update_matches(const String &text, Vec<Match> matches) {
        auto curr = text.begin();
        auto end = text.end();
        int idx = 0;

        for (auto &match: matches) {
            if (curr >= end) {
                break;
            }

            auto start = curr;

            while (idx < match.end) {
                char ch = utf8::next(curr, end);

                if (ch != 0) {
                    idx++;
                }

                if (idx == match.start) {
                    start = curr;
                }

                if (idx == match.end) {
                    match.text = String(start, curr);

                    Vec<Code> codes;

                    if (case_sensitive) codes = split_unicode(match.text.begin(), match.text.end());
                    else codes = lower(match.text.begin(), match.text.end());

                    match.data = metadata[hash(codes)].get();
                }
            }

        }

        return matches;
    }

    String replace(const String &text, Map<String, String> repl) {
        String norm_text = normalize(text);
        String output = "";
        Vec<Match> matches = extract(norm_text);

        auto idx = 0;
        auto curr = norm_text.begin();
        auto end = norm_text.end();

        for (auto match: matches) {
            if (curr >= end) {
                break;
            }

            auto start = curr;

            while (idx < match.start) {
                char ch = utf8::next(curr, end);

                if (ch != 0) {
                    idx++;
                }

                if (idx == match.start) {
                    output.append(String(start, curr));
                }
            }

            if (repl.count(match.text)) {
                output.append(repl[match.text]);
            } else {
                output.append(match.data->name);
            }

            while (idx < match.end) {
                char ch = utf8::next(curr, end);

                if (ch != 0) {
                    idx++;
                }
            }
        }

        if (curr < end) {
            output.append(String(curr, end));
        }

        return output;
    }

    // KeywordProcessor flashtext::make_processor(bool case_senitive) {
    //     return KeywordProcessor(case_senitive);
    // }
};
