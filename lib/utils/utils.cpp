#include <algorithm>
#include <locale>
#include <fstream>

#include "../shared/vocab.h"
#include "../shared/regex.h"

#include "utf8.h"
#include "utils.h"

using namespace vocab;
using namespace utils;


const Map<Code, String> CODE_TO_CHARS = []() {
    Map<Code, String> mapping;

    for (Vec<String> chars: { DIGITS, LOWER_CHARS }) {
        for (auto ch: chars) {
            Vec<Code> codes = chop(ch.begin(), ch.end());
            mapping[codes.front()] = ch;
        }
    }

    return mapping;
}();

template <typename Iter>
Vec<Code> utils::chop(Iter begin, Iter end) {
    Iter curr = begin;

    Vec<Code> codes;
    while (curr < end) {
        Code code = utf8::next(curr, end);

        if (code != 0) {
            codes.push_back(code);
        }
    }

    return codes;
}

Vec<Code> utils::chop(const String &str) {
    return chop(str.begin(), str.end());
}

String utils::trim(const String &str) {
    const auto begin = str.find_first_not_of(" ");
    const auto end = str.find_last_not_of(" ");

    if (begin == String::npos) return "";
    const auto range = end - begin + 1;

    return str.substr(begin, range);
}

Vec<String> utils::split(const String &str, const String &delimiter) {
    Vec<String> segments;

    auto last = 0U;
    auto next = 0U;
    while (last < str.size() && (next = str.find(delimiter, last)) != String::npos) {
        segments.push_back(str.substr(last, next - last));
        last += next + delimiter.size();
    }

    if (next + delimiter.size() < str.size()) {
        segments.push_back(str.substr(next + delimiter.size(), str.size()));
    }

    return segments;
}

String utils::join(const String &delimiter, Vec<String> segments) {
    String output;

    for (auto i = 0; i < segments.size() - 1; i++) {
        output.append(segments.at(i));
        output.append(delimiter);
    }

    if (segments.size() > 0) {
        output.append(segments.back());
    }

    return output;
}

String utils::lower(const String &str) {
    Vec<Code> codes = chop(str.begin(), str.end());
    std::setlocale(LC_ALL, "");
    String lower_str;

    for (Code code: codes) {
        String lower_ch;
        if (CODE_TO_CHARS.count(code)) {
            lower_ch = CODE_TO_CHARS.at(code);
            auto it = std::find(UPPER_CHARS.begin(), UPPER_CHARS.end(), lower_ch);
            if (it != UPPER_CHARS.end()) {
                lower_ch = LOWER_CHARS.at(it - UPPER_CHARS.begin()); 
            }
        } else {
            lower_ch = String(1, std::towlower(code));
        }
        lower_str.append(lower_ch);
    }
    
    return lower_str;
}

String utils::upper(const String &str) {
    Vec<Code> codes = chop(str.begin(), str.end());
    std::setlocale(LC_ALL, "");
    String upper_str;

    for (Code code: codes) {
        String upper_ch;
        if (CODE_TO_CHARS.count(code)) {
            upper_ch = CODE_TO_CHARS.at(code);
            auto it = std::find(LOWER_CHARS.begin(), LOWER_CHARS.end(), upper_ch);
            if (it != LOWER_CHARS.end()) {
                upper_ch = UPPER_CHARS.at(it - LOWER_CHARS.begin()); 
            }
        } else {
            upper_ch = String(1, std::towupper(code));
        }
        upper_str.append(upper_ch);
    }
    
    return upper_str;
}

Vec<String> utils::read(const String &path) {
    Vec<String> lines;

    std::ifstream input(path);

    String line;
    while (getline(input, line)) {
        lines.push_back(line);
    }

    return lines;
}

String utils::normalize(const String &str) {
    return trim(std::regex_replace(str, regex::SPACE, " "));
}
