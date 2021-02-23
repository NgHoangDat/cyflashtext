#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../shared/types.h"
#include "../utils/utils.h"

using namespace types;
using namespace utils;

namespace flashtext {

    struct Node;
    struct Match {
        int start;
        int end;
        String text;

        Match(int start, int end) {
            this->start = start;
            this->end = end;
        }
    };

    extern void put(const String &text, const String &name);
    extern void pop(const String &text, const String &name);
    extern Vec<Match> extract(const String &text, const String &name, bool strict);
}