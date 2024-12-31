#include "room_client.hpp"

bool Communication::RoomClient::connected()
{
    return connected_to_server;
}


void Communication::RoomClient::request_disconnection()
{
    if (host == nullptr || !connected_to_server) return;
    send(Networking::Message::Room::Client::REMOVE_PLAYER_REQUEST, &id, sizeof(size_t));
    enet_host_flush(host);
    destroy_host();
}

void Communication::RoomClient::request_disconnection(size_t client_id)
{
    if (host == nullptr || !connected_to_server) return;
    send(Networking::Message::Room::Client::REMOVE_PLAYER_REQUEST, &client_id, sizeof(size_t));
}

void Communication::RoomClient::request_spawn(std::function<void(Networking::Message::Room::SpawnData *)> callback)
{
    __spawn_data_callback = [callback](Networking::Message::Room::SpawnData * sd){
        callback(sd);
    };
    send(Networking::Message::Room::Client::REQUEST_SPAWN_DATA, &id, sizeof(size_t));
    
    enet_host_flush(host);
}

void Communication::RoomClient::request_game_update(void *pp, size_t psize)
{
    send(Networking::Message::Room::Client::GAME_STATE_UPDATE_REQUEST, pp, psize);
    enet_host_flush(host);
}

void Communication::RoomClient::request_secondary_update(void *pp, size_t psize)
{
    // Not needed for now
    enet_host_flush(host);
}

void Communication::RoomClient::reset_callbacks()
{
    room_broadcast_callback = {};
    set_map_callback = {};
    game_start_callback = {};
    disconnect_callback = {};
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

void Communication::RoomClient::send_map_id(int map)
{
    send(Networking::Message::Room::Client::MAP_SET_REQUEST, &map, sizeof(int));
    enet_host_flush(host);
}

void Communication::RoomClient::request_start()
{
    send(Networking::Message::Room::Client::START_GAME_REQUEST);
    enet_host_flush(host);
}

size_t Communication::RoomClient::get_id()
{
    return id;
}

std::pair<bool, std::string> Communication::RoomClient::handle_established_connection(const bool &cancel_requested)
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
                connected_to_server = true;
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

    switch(type){
        case ServerCommand::ROOM_LIST_BROADCAST:{
            char (*names)[Structs::NAME_SIZE] = (char (*)[Structs::NAME_SIZE])message;
            
            if (room_broadcast_callback){            
                std::vector<std::string> room_members;
                room_members.reserve(Structs::MAX_ROOM_SIZE);
                for (size_t i = 0; i < Structs::MAX_ROOM_SIZE; ++i){
                    room_members.emplace_back(names[i]);
                }
                room_broadcast_callback(room_members);
            }
            break;
        }
        case ServerCommand::DISCONNECT:{
            std::cout << "DC came in" << std::endl;
            destroy_host();
            connected_to_server = false;
            if (disconnect_callback){
                disconnect_callback();
            }
            break;
        }
        case ServerCommand::MAP_SET:{
            int map = *(int*)message;
            
            if (set_map_callback){
                set_map_callback(map);
            }
            break;
        }
        case ServerCommand::GAME_START:{           
            if (game_start_callback){
                game_start_callback();
            }
            break;
        }
        case ServerCommand::GAME_STATE_UPDATE:{
            if (game_update_callback){
                game_update_callback(message);
            }
            break;
        }
        case ServerCommand::SECONDARY_STATE_UPDATE:{
            if (secondary_update_callback){
                secondary_update_callback(message);
            }
            break;
        }
        case ServerCommand::SET_SPAWN_DATA:{
            if (__spawn_data_callback){
                __spawn_data_callback((Structs::SpawnData*)message);
                __spawn_data_callback = {};
            } else{
                std::cout << "SOMETHING VERY BAD HAS HAPPEND SIR" << std::endl;
            }
            break;
        }
    }
}

void Communication::RoomClient::on_stop()
{
    request_disconnection();
    if (__spawn_data_callback){
        __spawn_data_callback(nullptr);
        __spawn_data_callback = {};
    };
    connected_to_server = false;
}

void Communication::RoomClient::handle_random_disconnection()
{
    std::cout << "USER HAS BEEN RANDOMLY KICKED!" << std::endl;
    host = nullptr;
    destroy_host();
}
