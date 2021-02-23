#pragma once
#include "../utils/utils.h"

using namespace utils;

namespace tokenizer {
    Vec<String> tokenize(const String &str);
    Vec<String> join_sentences(Vec<String> tokens);
    String join_sentence(Vec<String> tokens);
    void recursive(Vec<String> tokens, const String &str, int begin, int end);
    
    template <typename T>
    void concatinate(Vec<T> target, Vec<Vec<T>> vectors);
}