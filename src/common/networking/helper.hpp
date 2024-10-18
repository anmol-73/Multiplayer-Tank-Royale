#ifndef HEADER_COMMON_NETWORKING_HELPER
#define HEADER_COMMON_NETWORKING_HELPER

#include <memory>
#include <string>
#include "void_ptr.hpp"
#include "string_algos.hpp"


namespace Networking
{
    /**
     * Encodes the message type and the message data into a single messages
     */
    std::pair<unique_void_ptr, size_t> encode_message(int type, void* data, size_t data_size);
    
    /**
     * Seperates the message type and the message data from an encoded message
     */
    std::pair<int, void*> decode_message(void* message);

    /**
     * Extracts the ip and the port from the address string given
     * NOTE: sets port to -1 if the address is not valid and returns reason of invalidity in pair.first
     */
    std::pair<std::string, int> parse_address_string(std::string& address);

} // namespace Networking

#endif