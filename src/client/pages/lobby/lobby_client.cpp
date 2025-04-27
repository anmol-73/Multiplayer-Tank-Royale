#include "lobby_client.hpp"
#include <cstring>
#define log(message) std::cout << "[Lobby] " << message << std::endl

bool ServiceConsumers::LobbyClient::is_connected()
{
    return _is_connected;
}

const std::vector<Communication::Lobby::RoomDetail> &ServiceConsumers::LobbyClient::get_rooms() const
{
    return rooms;
}

Communication::RequestStatus ServiceConsumers::LobbyClient::get_new_room_status()
{
    if (status == Communication::RequestStatus::DENIED){
        status = Communication::RequestStatus::IDLE;
        return Communication::RequestStatus::DENIED;
    }
    return status;
}

std::optional<Communication::Lobby::RoomDetail> ServiceConsumers::LobbyClient::get_new_room_detail()
{
    if (status != Communication::RequestStatus::ACCEPTED) return std::nullopt;
    return new_room_detail;
}

void ServiceConsumers::LobbyClient::request_new_room(const std::string &room_name)
{
    using namespace Communication::Lobby;

    assert(status == Communication::RequestStatus::IDLE);
    status = Communication::RequestStatus::ONGOING;
    
    RoomName name;
    strncpy(name, room_name.c_str(), sizeof(RoomName));

    send_message(Client::CREATE_ROOM, name, sizeof(RoomName));
}

void ServiceConsumers::LobbyClient::handle_message(Communication::Command type, const void *message, size_t size)
{
    if (!is_running()) return; // Ignore any incoming messages after the client has changed rooms
    using namespace Communication::Lobby;
    
    switch (static_cast<Server>(type)){
        case Server::CREATE_OK: {
            new_room_detail = *static_cast<const RoomDetail*>(message);
            status = Communication::RequestStatus::ACCEPTED;

            log("Room accepted! (" << new_room_detail.port << ")");
            break;
        }
        
        case Server::CREATE_DENIED: {
            status = Communication::RequestStatus::DENIED;
            log("Room denied :(");
            break;
        }
        
        case Server::ROOM_LIST: {
            size_t rooms_length = size / sizeof(RoomDetail);
            rooms.assign(
                static_cast<const RoomDetail*>(message),
                static_cast<const RoomDetail*>(message) + rooms_length
            );
            
            log("Active rooms count: " << rooms_length);
            break;
        }
    }
}

void ServiceConsumers::LobbyClient::on_start()
{
    status = Communication::RequestStatus::IDLE;
    log("Listening :)");
}

void ServiceConsumers::LobbyClient::on_finish()
{
    log("Stopped listening :(");
}

std::string ServiceConsumers::LobbyClient::on_connection_established()
{
    _is_connected = true;
    log("Connected!");
    return std::string();
}

void ServiceConsumers::LobbyClient::handle_disconnection()
{
    _is_connected = false;
    log("Disconnected!");
    ServiceConsumer::handle_disconnection();
}

#undef log