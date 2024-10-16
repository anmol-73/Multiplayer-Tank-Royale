#include "common.hpp"

std::pair<std::unique_ptr<void>, size_t> Networking::encode_message(int type, void *data, size_t data_size)
{
    size_t encoded_message_size = sizeof(type) + data_size;
    void* message_data = malloc(encoded_message_size);
    memcpy(message_data, &type, sizeof(int));
    memcpy((int*)message_data + 1, data, sizeof(data_size));
    return {std::unique_ptr<void>(message_data), encoded_message_size};
}

std::pair<int, void *> Networking::decode_message(void *message)
{
    int type = *(static_cast<int*>(message));
    void* data = (int*)message + 1;
    return {type, data};
}
