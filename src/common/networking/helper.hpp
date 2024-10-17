#ifndef HEADER_COMMON_NETWORKING_HELPER
#define HEADER_COMMON_NETWORKING_HELPER

#include <memory>

namespace Networking
{
    /**
     * Encodes the message type and the message data into a single messages
     */
    std::pair<std::unique_ptr<void>, size_t> encode_message(int type, void* data, size_t data_size);
    
    /**
     * Seperates the message type and the message data from an encoded message
     */
    std::pair<int, void*> decode_message(void* message);
} // namespace Networking

#endif