#include "message_encoding.hpp"

std::pair<Utils::unique_void_ptr, size_t> Communication::encode_message(Command type, const void *data, size_t data_sz)
{
    size_t encoded_message_size = sizeof(size_t) + data_sz;
    void* message_data = malloc(encoded_message_size);

    if (message_data == nullptr) throw std::runtime_error("Could not allocate memory to send data!");

    memcpy(message_data, &type, sizeof(Command)); // First 32 bits get the message type
    memcpy((Command*)message_data + 1, data, data_sz); // Fill in the rest of the data
    return {Utils::unique_void_ptr(message_data), encoded_message_size};
}

std::pair<Communication::Command, const void *> Communication::decode_message(const void *raw)
{
    size_t type = *(static_cast<const Command*>(raw)); // First 32 bits is the message type
    const void* data = static_cast<const Command*>(raw) + 1; // The next chunk of data is the message data itself
    return {type, data};
}
