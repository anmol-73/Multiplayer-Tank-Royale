#include "string_algos.hpp"

std::vector<std::string> StringAlgorithms::split(const std::string &string, const std::string &delim)
{
    std::vector<std::string> items;

    size_t last = 0; 
    size_t next = 0;
    while ((next = string.find(delim, last)) != std::string::npos) {
        items.push_back(string.substr(last, next - last));
        last = next + 1;
    }
    items.push_back(string.substr(last));
    return items;
}

void StringAlgorithms::strip(std::string &string)
{
    size_t i = 0;
    for (; i < string.size(); ++i){
        if (string[i] != ' ' && string[i] != '\n' && string[i] != '\t' && string[i] != '\r') break;
    }
    if (i != 0){
        string = string.substr(i);
    }
    while (!string.empty()){
        if (string[i] != ' ' && string[i] != '\n' && string[i] != '\t' && string[i] != '\r') break;
        string.pop_back();
    }
}

std::string StringAlgorithms::stripped(const std::string &string)
{
    size_t i = 0;
    for (; i < string.size(); ++i){
        if (string[i] != ' ' && string[i] != '\n' && string[i] != '\t' && string[i] != '\r') break;
    }

    size_t j = string.size();
    for (; j > 0; --j){
        if (string[j - 1] != ' ' && string[j - 1] != '\n' && string[j - 1] != '\t' && string[j - 1] != '\r') break;
    }

    return string.substr(i, j - i);
    
}

std::pair<bool, size_t> StringAlgorithms::sstoll(const std::string &string)
{
    try{
        return {true, std::stoll(string)};
    }
    catch(const std::exception& e){
        return {false, 0};
    }
}
