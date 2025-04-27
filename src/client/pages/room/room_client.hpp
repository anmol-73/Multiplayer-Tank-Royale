#ifndef H_PAGES_ROOM_CLIENT
#define H_PAGES_ROOM_CLIENT

#include <optional>

#include "communication/protocol.hpp"
#include "service_consumer.hpp"

#include <mutex>
#include <condition_variable>


namespace ServiceConsumers
{
    struct RoomClient: public ServiceConsumer{
    public:
        bool is_connected();
        
        Communication::RequestStatus get_join_status();

        /**
         * NOTE: Once called sets the status to idle! (To allow for future name set requests)
         */
        Communication::RequestStatus get_name_set_status();

        int get_client_id();
        std::optional<int> get_game_port();

        const Communication::Room::RoomSettings& get_current_settings() const;
        const std::vector<Communication::Room::PlayerDetail>& get_joined_players() const;

        std::string get_government_name();
        void request_new_name(const std::string& new_name);
        void request_game_start();
        void request_set_settings(const Communication::Room::RoomSettings& new_settings);
        
    protected:
        void handle_message(Communication::Command type, const void *message, size_t size);
        void on_start();
        void on_finish();
        std::string on_connection_established();
        void handle_disconnection();
        
    private:
        bool _is_connected = false;
        Communication::RequestStatus join_status = Communication::RequestStatus::ONGOING;
        Communication::RequestStatus name_set_status = Communication::RequestStatus::IDLE;
        Communication::RequestStatus game_start_status = Communication::RequestStatus::IDLE;

        Communication::Room::RoomSettings settings;
        std::vector<Communication::Room::PlayerDetail> players;
        
        int assigned_id = -1;
        int game_port = -1;
    };
} // namespace Pages

#endif