#include <memory>

namespace Networking
{
    std::pair<std::unique_ptr<void>, size_t> encode_message(int type, void* data, size_t data_size);
    std::pair<int, void*> decode_message(void* message);
} // namespace Networking
