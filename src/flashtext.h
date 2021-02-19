#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace flashtext {
    using String = std::string;
    using Code = unsigned int;

    template <typename T>
    using Vec = std::vector<T>;

    template <typename T>
    using Set = std::unordered_set<T>;

    template <typename K, typename V>
    using Map = std::unordered_map<K, V>;

    struct Node;
    struct Metadata;
    struct Match;
    struct KeywordProcessor;

    template <typename Iter>
    Vec<Code> split_unicode(Iter begin, Iter end);

    template <typename Iter>
    Vec<Code> lower(Iter begin, Iter end);

    String hash(Vec<Code> codes);

    String substring(const String &str, int start, int end);
    String trim(const String &str);
    String normalize(const String &str);
    // KeywordProcessor make_processor(bool case_sensitive);
}
