#pragma once

#include "../utils/utils.h"
#include "../shared/vocab.h"

using namespace utils;

namespace vocab::tokenizer {
    Map<String, String> synonyms;
    Set<String> common_words;
    Set<String> company_names;
    Set<String> short_country_names;
    Set<String> long_country_names;
    Set<String> location_names;
    Set<String> sentence_prefixes;
    Set<String> middle_names;
    Set<String> family_names;
    Set<String> abbreviation;
    Set<String> exceptions;
    Set<String> BRACES;
    Vec<String> PUNCTUATIONS;
}