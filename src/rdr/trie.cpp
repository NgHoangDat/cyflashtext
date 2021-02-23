#include <memory>

#include "trie.h"

using namespace utils;
using namespace trie;


struct trie::FWObject {
    Array<String, 10> context;

    FWObject(bool check) {
        if (check) {
            for (size_t i = 0; i < context.size(); i += 2) {
                context[i] = "<W>";
                context[i + 1] = "<T>";
            }
        }
    }
};

struct trie::WordTag {
    String word;
    String tag;
    String form;

    WordTag(String word, String tag) {
        this->form = word;
        this->word = lower(word);
        this->tag = tag;
    }
};

struct trie::Node {
    std::shared_ptr<FWObject> condition;
    String conclusion;
    std::shared_ptr<Node> except_node;
    std::shared_ptr<Node> if_not_node;
    std::shared_ptr<Node> father_Node;
    int depth;

    int count_nodes();
    bool is_satisfy(const FWObject &obj);
};


FWObject trie::get_condition(const String &str) {
    FWObject condition(false);

    for (auto rule: split(str, " and ")) {
        rule = trim(rule);
        String key = str.substr(rule.find_first_of(".") + 1, rule.find_first_of(" "));
        String val = [str]() -> String {
            if (str.find("\"\"")) {
                if (str.find("Word")) {
                    return "<W>";
                }
                return "<T>";
            }
            return str.substr(str.find_first_of("\"") + 1, str.size() - 1);
        }();

        if (key.compare("prevWord2") == 0) {
            condition.context[4] = val;
            continue;
        }
        
        if (key.compare("prevTag2") == 0) {
            condition.context[5] = val;
            continue;
        } 
        
        if (key.compare("prevWord1") == 0) {
            condition.context[2] = val;
            continue;
        }
        
        if (key.compare("prevTag1") == 0) {
            condition.context[3] = val;
            continue;
        } 
        
        if (key.compare("word") == 0) {
            condition.context[1] = val;
            continue;
        } 
        
        if (key.compare("tag") == 0) {
            condition.context[0] = val;
            continue;
        } 
        
        if (key.compare("nextWord1") == 0) {
            condition.context[6] = val;
            continue;
        } 
        
        if (key.compare("nextTag1") == 0) {
            condition.context[7] = val;
            continue;
        }
        
        if (key.compare("nextWord2") == 0) {
            condition.context[8] = val;
            continue;
        } 
        
        if (key.compare("nextTag2") == 0) {
            condition.context[9] = val;
            continue;
        }
    }
}

FWObject trie::get_object(Vec<WordTag> word_tags, size_t size, size_t index) {
    FWObject obj(true);

    if (index > 1) {
        obj.context[4] = word_tags.at(index - 2).word;
        obj.context[5] = word_tags.at(index - 2).tag;
    }

    if (index > 0) {
        obj.context[2] = word_tags.at(index - 1).word;
        obj.context[3] = word_tags.at(index - 1).tag;
    }

    String curr_word = word_tags.at(index).word;
    String curr_tag = word_tags.at(index).tag;

    obj.context[1] = curr_word;
    obj.context[0] = curr_tag;

    if (index < size - 1) {
        obj.context[6] = word_tags.at(index + 1).word;
        obj.context[7] = word_tags.at(index + 1).tag;
    }

    if (index < size - 2) {
        obj.context[8] = word_tags.at(index + 2).word;
        obj.context[9] = word_tags.at(index + 2).tag;
    }

    return obj;
}

int Node::count_nodes() {
    int count = 1;
    if (except_node != nullptr) {
        count += except_node.get()->count_nodes();
    }

    if (if_not_node != nullptr) {
        count += if_not_node.get()->count_nodes();
    }

    return count;
}

bool Node::is_satisfy(const FWObject &obj) {
    auto check = true;

    for (auto i = 0; i < condition.get()->context.size(); i++) {
        String key = condition.get()->context.at(i);

        if (!key.empty()) {
            if (key.compare(obj.context.at(i))) {
                check = false;
                break;
            }
        }
    }

    return check;
}