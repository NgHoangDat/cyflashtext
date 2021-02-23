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

    struct Match {
        int start;
        int end;
        String text;

        Match(int start, int end) {
            this->start = start;
            this->end = end;
        }
    };

    struct Node {
        Map<Code, std::shared_ptr<Node>> next;
        bool flag = false;

        template <typename Iter>
        void put(Iter begin, Iter end);

        void put_key(const String &text);

        template <typename Iter>
        void pop(Iter begin, Iter end);

        void pop_key(const String &text);

        Vec<Match> extract(const String &text, bool strict);

        Vec<Match> update_matches(const String &text, Vec<Match> matches);
    };
}
