#include "structs.hpp"

Communication::Address::Address(const std::string &address)
{
    using namespace Utils::StringAlgorithms;

    std::string ip;
    auto ip_and_port = split(stripped(address), ":");
    
    if (ip_and_port.size() != 2){
        name = "Address needs to specify port!";
        port = -1;
        return;
    }
    
    strip(ip_and_port[0]);
    strip(ip_and_port[1]);
    
    auto ip_seperated = split(ip_and_port[0], ".");
    // The ip must have exactly 4 segments!
    if (ip_seperated.size() != 4){
        name = "IP Address needs to have 4 segments!";
        port = -1;
        return;
    }
    for (std::string& segment: ip_seperated){
        auto [valid, num] = sstoll(segment);
        
        if (!valid){
            name = "IP Address needs to have integer segments!";
            port = -1;
            return;
        }
        if (num > 255){
            name = "IP Address needs to have integer segments <= 255!";
            port = -1;
            return;
        }
    }
    auto [valid, _port] = sstoll(ip_and_port[1]);
    if (!valid){
        name = "Port must be a valid integer!";
        _port = -1;
        return;
    }

    name = ip_and_port[0];
    port = _port;
    return;
}

Communication::Address::Address()
{
    port = -1;
}

bool Communication::Address::is_invalid()
{
    return port < 0;
}
