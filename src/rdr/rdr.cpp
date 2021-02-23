#include "rdr.h"
#include "../utils/utils.h"

using namespace utils;
using namespace rdr;

const Map<String, String> NORMALIZER = Map<String, String>({
    {"òa", "oà"},
    {"óa", "oá"},
    {"ỏa", "oả"},
    {"õa", "oã"},
    {"ọa", "oạ"},
    {"òe", "oè"},
    {"óe", "oé"},
    {"ỏe", "oẻ"},
    {"õe", "oẽ"},
    {"ọe", "oẹ"},
    {"ùy", "uỳ"},
    {"úy", "uý"},
    {"ủy", "uỷ"},
    {"ũy", "uỹ"},
    {"ụy", "uỵ"},
    {"Ủy", "Uỷ"}
});

const Set<String> NORMALIZER_KEYS = []() {
    Set<String> keys;

    for (auto item: NORMALIZER) {
        keys.insert(item.first);
    }

    return keys;
}();


