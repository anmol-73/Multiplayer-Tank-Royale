#ifndef H_COM_MSG_ENCODING
#define H_COM_MSG_ENCODING

#include <memory>
#include <string>
#include <stdexcept>

#include "misc/void_ptr.hpp"

namespace Communication
{
    using Command = int;

    /**
     * Encodes the message type and the message data into a single messages
     */
    std::pair<Utils::unique_void_ptr, size_t> encode_message(Command type, const void *data, size_t data_sz);
    
    /**
     * Seperates the message type and the message data from an encoded message
     */
    std::pair<Command, const void *> decode_message(const void *raw);
} // namespace Communication


#endif