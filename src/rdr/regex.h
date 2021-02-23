#pragma once

#include "../shared/types.h"
#include "../shared/regex.h"

namespace regex::tokenizer {
    const Regex ELLIPSIS("\\.{2,}");

    const Regex EMAIL("([\\w\\d_\\.-]+)@(([\\d\\w-]+)\\.)*([\\d\\w-]+)");

    const Regex FULL_DATE("(0?[1-9]|[12][0-9]|3[01])(\\/|-|\\.)(1[0-2]|(0?[1-9]))((\\/|-|\\.)\\d{4})");

    const Regex MONTH("(1[0-2]|(0?[1-9]))(\\/)\\d{4}");

    const Regex DATE("(0?[1-9]|[12][0-9]|3[01])(\\/)(1[0-2]|(0?[1-9]))");

    const Regex TIME("(\\d\\d:\\d\\d:\\d\\d)|((0?\\d|1\\d|2[0-3])(:|h)(0?\\d|[1-5]\\d)(’|'|p|ph)?)");

    const Regex MONEY("\\p{Sc}\\d+([\\.,]\\d+)*|\\d+([\\.,]\\d+)*\\p{Sc}");

    const Regex PHONE_NUMBER("(\\(?\\+\\d{1,2}\\)?[\\s\\.-]?)?\\d{2,}[\\s\\.-]?\\d{3,}[\\s\\.-]?\\d{3,}");

    const Regex URL("(((https?|ftp):\\/\\/|www\\.)[^\\s/$.?#].[^\\s]*)|(https?:\\/\\/)?(www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)");

    const Regex NUMBER("[-+]?\\d+([\\.,]\\d+)*%?\\p{Sc}?");

    const Regex PUNCTUATION(",|\\.|:|\\?|!|);|-|_|\"|'|“|”|\\||\\(|\\)|\\[|\\]|\\{|\\}|âŸ¨|âŸ©|Â«|Â»|\\\\|\\/|\\â€˜|\\â€™|\\â€œ|\\â€�|â€¦|…|‘|’|·");

    const Regex SPECIAL_CHAR("\\~|\\@|\\#|\\^|\\&|\\*|\\+|\\-|\\â€“|<|>|\\|");

    const Regex EOS_PUNCTUATION("(\\.+|\\?|!|…)");

    const Regex NUMBERS_EXPRESSION("[-+]?\\d+([\\.,]\\d+)*%?\\p{Sc}?([\\+\\-\\*\\/][-+]?\\d+([\\.,]\\d+)*%?\\p{Sc}?)*");

    const Regex SHORT_NAME("([\\p{L}]+([.\\-][\\p{L}]+)+)|([\\p{L}]+-\\d+)");

    const Regex ALL_CAP("[A-Z]+\\.[A-Z]+");

    const Map<String, Regex> REGEXES = {
        {"ellipsis", ELLIPSIS},
        {"email", EMAIL},
        {"url", URL},
        {"full_date", FULL_DATE},
        {"month", MONTH},
        {"date", DATE},
        {"time", TIME},
        {"money", MONEY},
        {"phone_number", PHONE_NUMBER},
        {"short_name", SHORT_NAME},
        {"numbers_expression", NUMBERS_EXPRESSION},
        {"number", NUMBER},
        {"punctuation", PUNCTUATION},
        {"special_char", SPECIAL_CHAR},
        {"all_cap", ALL_CAP}
    };

}