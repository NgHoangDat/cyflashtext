#include <array>
#include <memory>

#include "../utils/utf8.h"
#include "../shared/vocab.h"

#include "flashtext.h"

using namespace vocab;
using namespace flashtext;


const Set<Code> NON_BOUNDARY_CODE = []() {
    const Vec<String> SPECIAL_CHARS = {"_"};    
    
    Set<Code> non_boundary_code;
    
    for (Vec<String> chars: { SPECIAL_CHARS, DIGITS, LOWER_CHARS, UPPER_CHARS }) {
        for(auto val: chars) {
            auto it = val.begin();
            Code code = utf8::next(it, val.end());
            non_boundary_code.insert(code);
        }
    }

    return non_boundary_code;
}();

Map<String, std::shared_ptr<Node>> NODES;

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

    void put(const String &text) {
        Vec<Code> codes = chop(text.begin(), text.end());
        this->put(codes.begin(), codes.end());
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

    void pop(const String &text) {
        Vec<Code> codes = chop(text.begin(), text.end());
        this->pop(codes.begin(), codes.end());    
    }

    Vec<Match> extract(const String &text, bool strict=true) {
        Vec<Code> codes = chop(text.begin(), text.end());

        if (codes.size() == 0) {
            return {};
        }

        auto curr = this;
        auto start = 0;
        auto end = 0;
        auto reset = false;
        auto size = codes.size();
        unsigned int idx = 0;

        Vec<Match> matches;

        while (idx < size) {
            auto code = codes.at(idx);

            if (!strict || !NON_BOUNDARY_CODE.count(code)) {
                if (curr->flag || curr->next.count(code)) {
                    unsigned int seq_start = 0;
                    unsigned int seq_end = 0;
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

                            if (!strict || !NON_BOUNDARY_CODE.count(inner)) {
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
                        if (seq_end > size) {
                            seq_end = size;
                        }

                        matches.push_back(Match(seq_start, seq_end));
                    }
                }

                curr = this;
                reset = true;
            } else {
                if (curr->next.count(code)) {
                    curr = curr->next[code].get();
                } else {
                    curr = this;
                    reset = true;

                    auto idy = idx + 1;

                    while (!strict || idy < size) {
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
                    matches.push_back(Match(start, size));
                }
            }

            idx++;

            if (reset) {
                reset = false;
                start = idx;
            }
        }

        return update_matches(text, matches);
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

            while (idx < match.end && curr < end) {
                char ch = utf8::next(curr, end);

                if (ch != 0) {
                    idx++;
                }

                if (idx == match.start) {
                    start = curr;
                }
            }

            if (idx == match.end) {
                match.text = String(start, curr);
            } else {
                match.text = String(start, curr + 1);
            }
        }

        return matches;
    }
};

void flashtext::put(const String &text, const String &name="default") {
    if (!NODES.count(name)) {
        NODES[name] = std::make_shared<Node>(Node());
    }
    NODES[name].get()->put(text);
}

void flashtext::pop(const String &text, const String &name="default") {
    if (NODES.count(name)) {
        NODES[name].get()->pop(text);
    }
}

Vec<Match> flashtext::extract(const String &name, const String &text="default", bool strict=true) {
    if (!NODES.count(name)) {
        return {};
    }

    return NODES[name].get()->extract(text, strict);
}

int main() {
    Node trie;
    trie.put("on");
    Vec<Match> x = trie.extract("asf on dsgon", true);
    Vec<Match> y = trie.extract("asf on dsgon", false);
    return 0;
}