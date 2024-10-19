#include "helper.hpp"
#include "iostream"

std::pair<unique_void_ptr, size_t> Networking::encode_message(size_t type, void *data, size_t data_size)
{
    size_t encoded_message_size = sizeof(size_t) + data_size;
    void* message_data = malloc(encoded_message_size);

    memcpy(message_data, &type, sizeof(size_t)); // First 64 bits get the message type
    memcpy((size_t*)message_data + 1, data, data_size); // Fill in the rest of the data
    return {unique_void_ptr(message_data), encoded_message_size};
}

std::pair<size_t, void *> Networking::decode_message(void *message)
{
    size_t type = *(static_cast<size_t*>(message)); // First 64 bits is the message type
    void* data = static_cast<size_t*>(message) + 1; // The next chunk of data is the message data itself
    return {type, data};
}

std::pair<std::string, int> Networking::parse_address_string(std::string &address)
{
    std::string ip;
    auto ip_and_port = StringAlgorithms::split(address, ":");
    
    if (ip_and_port.size() != 2){
        return {"Address needs to specify port!", -1};
    }
    
    StringAlgorithms::strip(ip_and_port[0]);
    StringAlgorithms::strip(ip_and_port[1]);
    
    auto ip_seperated = StringAlgorithms::split(ip_and_port[0], ".");
    // The ip must have exactly 4 segments!
    if (ip_seperated.size() != 4){
        return {"IP Address needs to have 4 segments!", -1};
    }
    for (std::string& segment: ip_seperated){
        auto [valid, num] = StringAlgorithms::sstoll(segment);
        
        if (!valid){
            return {"IP Address needs to have integer segments!", -1};
        }
        if (num > 255){
            return {"IP Address needs to have integer segments <= 255!", -1};
        }
    }
    auto [valid, port] = StringAlgorithms::sstoll(ip_and_port[1]);
    if (!valid){
        return {"Port must be a valid integer!", -1};
    }

    return {ip_and_port[0], port};
}

