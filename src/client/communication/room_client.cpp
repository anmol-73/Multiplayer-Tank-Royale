#include "room_client.hpp"

inline bool Communication::RoomClient::connected()
{
    return connected_to_server;
}

void Communication::RoomClient::send_name_request(const std::string &name)
{
    assert(name.size() < Networking::Message::Room::NAME_SIZE);
    
    Networking::Message::Room::NameSetRequest req = {
        .client_id = id
    };
    strcpy(req.name, name.c_str());
    
    send(Networking::Message::Room::Client::NAME_SET_REQUEST, &req, sizeof(Networking::Message::Room::NameSetRequest));
    enet_host_flush(host);
}

std::pair<bool, std::string> Communication::RoomClient::handle_established_connection(const bool& cancel_requested)
{
    const size_t timeout = 500;
    const size_t max_tries = 10;
    ENetEvent event;
    
    for (size_t tries = 0; tries < max_tries; ++ tries){
        if (cancel_requested) return {false, "Connection request cancelled!"};
        if (enet_host_service(host, &event, timeout) > 0 && event.type == ENET_EVENT_TYPE_RECEIVE){
            auto [type, data] = Networking::decode_message(event.packet->data);
            
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
    namespace Structs = Networking::Message::Room;
    using ServerCommand = Structs::Server;
    using ClientCommand = Structs::Client;

    switch(type){
        case ServerCommand::ROOM_LIST_BROADCAST:
            char names[Structs::MAX_ROOM_SIZE][Structs::NAME_SIZE];
            memcpy(names, message, sizeof(char) * Structs::NAME_SIZE * Structs::MAX_ROOM_SIZE);
            
            for (size_t i = 0; i < Structs::MAX_ROOM_SIZE; ++i){
                std::cout << i << " " << names[i] << std::endl;   
            }
    }
}

void Communication::RoomClient::on_stop()
{
    connected_to_server = false;
}
