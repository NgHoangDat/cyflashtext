#pragma once

#include "../shared/types.h"

using namespace types;

namespace utils {
    template <typename Iter>
    Vec<Code> chop(Iter begin, Iter end);

    Vec<Code> chop(const String &str);

    Vec<String> split(const String &str, const String &delimiter);

    String join(const String &delimiter, Vec<String> segments);

    String trim(const String &str);

    String lower(const String &str);

    String upper(const String &str);

    Vec<String> read(const String &path);

    String normalize(const String &str);

    bool starts_with(const String &str, const String &prefix);

    bool ends_with(const String &str, const String &suffix);
}