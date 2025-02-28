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

        enum NewRoomStatus{
            DENIED,
            ONGOING,
            ACCEPTED
        };

        std::optional<Communication::Lobby::RoomDetail> get_new_room_detail();
        void request_new_room(const std::string& room_name);
        NewRoomStatus get_new_room_status();
        
    protected:
        virtual void handle_message(Communication::Command type, const void *message, size_t size);
        virtual void on_start();
        virtual void on_finish();
        virtual std::string on_connection_established();
        virtual void handle_disconnection();
        
    private:
        bool _is_connected = false;
        std::vector<Communication::Lobby::RoomDetail> rooms;

        NewRoomStatus status = NewRoomStatus::DENIED;
        Communication::Lobby::RoomDetail new_room_detail = {0};
    };
} // namespace Pages

#endif