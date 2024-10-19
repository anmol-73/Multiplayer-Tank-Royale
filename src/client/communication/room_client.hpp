#ifndef HEADER_COMMUNICATION_ROOM_CLIENT
#define HEADER_COMMUNICATION_ROOM_CLIENT

#include <memory>
#include <stdexcept>
#include <map>
#include <vector>
#include "client.hpp"

namespace Communication
{
    struct RoomClient: public Client{
    public:
        inline bool connected();
    private:
        std::pair<bool, std::string> handle_established_connection(const bool& cancel_requested);
        void handle_message(size_t type, void* message);
        void on_stop();

        size_t id;
        bool connected_to_server = false;
    };
} // namespace Communication


#endif