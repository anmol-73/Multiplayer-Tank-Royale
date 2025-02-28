#ifndef H_COM_SA
#define H_COM_SA

#include <vector>
#include <string>

namespace Utils
{
    namespace StringAlgorithms
    {
        std::vector<std::string> split(const std::string& string, const std::string& delim);
        void strip(std::string& string);
        std::string stripped(const std::string& string);
        std::pair<bool, size_t> sstoll(const std::string &string);
    } // namespace StringAlgorithms
    
} // namespace Utils

#endif