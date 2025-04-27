#ifndef H_PAGES_LOBBY_CLIENT
#define H_PAGES_LOBBY_CLIENT

#include <optional>

#include "communication/protocol.hpp"
#include "service_consumer.hpp"

namespace ServiceConsumers
{
    struct LobbyClient: public ServiceConsumer{
    public:
        bool is_connected();
        const std::vector<Communication::Lobby::RoomDetail>& get_rooms() const;

        std::optional<Communication::Lobby::RoomDetail> get_new_room_detail();
        void request_new_room(const std::string& room_name);
        Communication::RequestStatus get_new_room_status();
        
    protected:
        void handle_message(Communication::Command type, const void *message, size_t size);
        void on_start();
        void on_finish();
        std::string on_connection_established();
        void handle_disconnection();
        
    private:
        bool _is_connected = false;
        std::vector<Communication::Lobby::RoomDetail> rooms;

        Communication::RequestStatus status = Communication::RequestStatus::DENIED;
        Communication::Lobby::RoomDetail new_room_detail = {0};
    };
} // namespace Pages

#endif