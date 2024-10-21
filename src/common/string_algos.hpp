#include <string>
#include <vector>

namespace StringAlgorithms
{
    std::vector<std::string> split(const std::string& string, const std::string& delim);
    void strip(std::string& string);
    std::string stripped(const std::string& string);
    std::pair<bool, size_t> sstoll(const std::string &string);
} // namespace StringAlgorithms
