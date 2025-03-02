#include "room_client.hpp"

#define log(message) std::cout << "[Room] " << message << std::endl

bool ServiceConsumers::RoomClient::is_connected()
{
    return _is_connected;
}

Communication::RequestStatus ServiceConsumers::RoomClient::get_join_status()
{
    return join_status;
}

Communication::RequestStatus ServiceConsumers::RoomClient::get_name_set_status()
{
    auto nss = name_set_status;
    if (nss == Communication::RequestStatus::ONGOING) return nss;
    name_set_status = Communication::RequestStatus::IDLE;
    
    return nss;
}

int ServiceConsumers::RoomClient::get_client_id()
{
    assert(assigned_id != -1);
    return assigned_id;
}

std::optional<int> ServiceConsumers::RoomClient::get_game_port()
{
    if (game_port == -1) return std::nullopt;

    return game_port;
}

const Communication::Room::RoomSettings &ServiceConsumers::RoomClient::get_current_settings() const
{
    return settings;
}

const std::vector<Communication::Room::PlayerDetail> &ServiceConsumers::RoomClient::get_joined_players() const
{
    return players;
}

std::string ServiceConsumers::RoomClient::get_government_name()
{
    for (size_t i = 0; i < players.size(); ++i){
        if (players[i].id == assigned_id){
            return players[i].name;
        }
    }

    assert(false);
}

void ServiceConsumers::RoomClient::request_new_name(const std::string &new_name)
{
    using namespace Communication::Room;

    assert(name_set_status == Communication::RequestStatus::IDLE);
    name_set_status = Communication::RequestStatus::ONGOING;
    
    PlayerName name;
    strncpy(name, new_name.c_str(), sizeof(PlayerName));

    send_message(Client::SET_NAME, name, sizeof(PlayerName));
}

void ServiceConsumers::RoomClient::request_game_start()
{
    using namespace Communication::Room;
    
    if (game_start_status == Communication::RequestStatus::ONGOING) return;
    game_start_status = Communication::RequestStatus::ONGOING;
    
    send_command(Client::START_GAME);
}

void ServiceConsumers::RoomClient::request_set_settings(const Communication::Room::RoomSettings &new_settings)
{
    using namespace Communication::Room;
    settings = new_settings;
    send_message(Client::SET_SETTINGS, &new_settings, sizeof(RoomSettings));
}

void ServiceConsumers::RoomClient::handle_message(Communication::Command type, const void *message, size_t size)
{
    if (!is_running()) return; // Ignore any incoming messages after the client has changed rooms
    using namespace Communication::Room;
    
    switch (static_cast<Server>(type)){
        case Server::JOIN_OK: {
            assigned_id = *static_cast<const int *>(message);
            log("Joined room with ID: " << assigned_id);
            join_status = Communication::RequestStatus::ACCEPTED;
            
            break;
        }
        
        case Server::JOIN_DENIED: {
            log("Room denied join!");
            join_status = Communication::RequestStatus::DENIED;
 
            break;
        }

        case Server::JOIN_GAME: {
            game_port = *static_cast<const int *>(message);
            log("Game created! (" << game_port << ")");
            
            break;
        }

        case Server::NAME_OK: {
            log("Name accepted :)");
            name_set_status = Communication::RequestStatus::ACCEPTED;
            break;
        }
        
        case Server::NAME_DENIED: {
            log("Name denied :(");
            name_set_status = Communication::RequestStatus::DENIED;
            break;
        }
        
        case Server::PLAYER_LIST: {
            size_t players_length = size / sizeof(PlayerDetail);
            players.assign(
                static_cast<const PlayerDetail*>(message),
                static_cast<const PlayerDetail*>(message) + players_length
            );
            
            log("Connected players count: " << players_length);
            break;
        }
        
        case Server::SETTINGS_SET: {
            log("Room settings updated...");
            settings = *static_cast<const RoomSettings*>(message);
            
            break;
        }
    }
}

void ServiceConsumers::RoomClient::on_start()
{
    join_status = Communication::RequestStatus::ONGOING;
    name_set_status = Communication::RequestStatus::IDLE;
    log("Listening :)");
}

void ServiceConsumers::RoomClient::on_finish()
{
    log("Stopped listening :(");
}

std::string ServiceConsumers::RoomClient::on_connection_established()
{
    _is_connected = true;
    log("Connected!");
    return std::string();
}

void ServiceConsumers::RoomClient::handle_disconnection()
{
    _is_connected = false;
    log("Disconnected!");
    ServiceConsumer::handle_disconnection();
}

#undef log