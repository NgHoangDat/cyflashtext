#pragma once

#include "../utils/utils.h"

using namespace utils;

namespace trie {
    struct FWObject;
    struct WordTag;
    struct Node;

    FWObject get_condition(const String &str);
    FWObject get_object(Vec<WordTag> word_tags, size_t size, size_t index);
}