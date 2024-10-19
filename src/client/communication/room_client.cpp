#include "room_client.hpp"

inline bool Communication::RoomClient::connected()
{
    return connected_to_server;
}

std::pair<bool, std::string> Communication::RoomClient::handle_established_connection(const bool& cancel_requested)
{
    const size_t timeout = 300;
    const size_t max_tries = 10;
    ENetEvent event;
    
    for (size_t tries = 0; tries < max_tries; ++ tries){
        if (cancel_requested) return {false, "Connection request cancelled!"};
        if (enet_host_service(host, &event, timeout) > 0){
            auto [type, data] = Networking::decode_message(event.packet->data);
            
            std::cout << type << " " << event.packet->dataLength << std::endl;
            if (type == Networking::Message::Room::Server::CONNECT_DENIED){
                return {false, std::string((char*)data)};
            }
            if (type == Networking::Message::Room::Server::CONNECT_OK){
                id = *(size_t*)data;
                return {true, ""};
            }
            
            return {false, "Server did not follow correct protocol!"};
        }
    }
    return {false, "Server did not respond in time!"};
}

void Communication::RoomClient::handle_message(size_t type, void *message)
{

}

void Communication::RoomClient::on_stop()
{
    connected_to_server = false;
}
