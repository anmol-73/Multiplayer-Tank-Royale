#ifndef H_COM_STRUCTS
#define H_COM_STRUCTS

#include <string>

namespace Communication
{
    struct Address{
        std::string name;
        int port;
    };    
} // namespace Communication

#endif