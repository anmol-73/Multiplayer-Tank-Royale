#ifndef HEADER_COMMUNICATION_ROOM_CLIENT
#define HEADER_COMMUNICATION_ROOM_CLIENT

#include <cassert>
#include <memory>
#include <stdexcept>
#include <map>
#include <functional>
#include <vector>
#include "client.hpp"

namespace Communication
{
    struct RoomClient: public Client{
    public:
        inline bool connected();
        void send_name_request(const std::string& name);
        void send_map_id(int map);
        void request_start();
        void request_disconnection();
        void request_disconnection(size_t client_id);

        void reset_callbacks();
        std::function<void(std::vector<std::string>)> room_broadcast_callback;
        std::function<void(int)> set_map_callback;
        std::function<void()> game_start_callback;
        std::function<void()> disconnect_callback;

        size_t get_id();
    private:
        std::pair<bool, std::string> handle_established_connection(const bool& cancel_requested);
        void handle_message(size_t type, void* message);
        void on_stop();

        size_t id;
        bool connected_to_server = false;
    };
} // namespace Communication


#endif