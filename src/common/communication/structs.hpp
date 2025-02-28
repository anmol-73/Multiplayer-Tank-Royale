#ifndef H_COM_STRUCTS
#define H_COM_STRUCTS

#include "misc/string_algorithms.hpp"

namespace Communication
{
    struct Address{
    public:
        Address(const std::string& address);
        Address(); // Creates an invalid address

        std::string name;
        int port;

        bool is_invalid();
    };    
} // namespace Communication

#endif