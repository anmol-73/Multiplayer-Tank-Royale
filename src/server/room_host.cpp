#include "room_host.hpp"

RoomHost::RoomHost()
{
    members.assign(Networking::Message::Room::MAX_ROOM_SIZE, {});
    for (size_t i = 0; i < Networking::Message::Room::MAX_ROOM_SIZE; ++i){
        strcpy(names[i], "");
    }
}

void RoomHost::handle_new_client(ENetPeer *peer)
{
    namespace Structs = Networking::Message::Room;
    using ServerCommand = Structs::Server;
    if (current_room_size >= Structs::MAX_ROOM_SIZE){
        char reason[Structs::STRING_MESSAGE_SIZE] = "Room is full!";
        send(ServerCommand::CONNECT_DENIED, reason, sizeof(char) * Structs::STRING_MESSAGE_SIZE, peer);
        
        enet_host_flush(host);
        enet_peer_reset(peer);
        return;
    };

    ++current_room_size;
    for (size_t i = 0; i < members.size(); ++i){
        if (members[i] == nullptr){
            members[i] = peer;
            std::cout << "Client(" << i << ") has connected!" << std::endl;
            send(Networking::Message::Room::Server::CONNECT_OK, &i, sizeof(size_t), peer);
            return;
        }
    }

    throw std::runtime_error("Critical logic error! Could not assign ID to room member");
}

void RoomHost::handle_disconnection(ENetPeer *peer)
{
    namespace Structs = Networking::Message::Room;
    using ServerCommand = Structs::Server;
    for (size_t i = 0; i < members.size(); ++i){
        // Handle the case where the client didn't properly disconnect also :)
        if (members[i] == peer){
            members[i] = nullptr;
            enet_peer_reset(peer);
            strcpy(names[i], "");
            std::cout << "Client(" << i << ") has forcibly disconnected!" << std::endl;
            if (1){ // Update the players connected
                send(ServerCommand::ROOM_LIST_BROADCAST, names, sizeof(char) * Structs::NAME_SIZE * Structs::MAX_ROOM_SIZE);
                enet_host_flush(host);
            }
            --current_room_size;
            break;
        }
    }
}

void RoomHost::handle_message(ENetPeer *peer, size_t type, void *message)
{
    // NOTE: Use enet_host_flush in order to send the message immediately!
    namespace Structs = Networking::Message::Room;
    using ServerCommand = Structs::Server;
    using ClientCommand = Structs::Client;
    // std::cout << type << std::endl;
    switch (type)
    {
        case ClientCommand::NAME_SET_REQUEST:
        {
            Structs::NameSetRequest *name = (Structs::NameSetRequest*)message;
            strncpy(names[name->client_id], name->name, Structs::NAME_SIZE);
            for (size_t i = 0; i < Structs::MAX_ROOM_SIZE; ++i){
                std::cout << i << " " << names[i] << std::endl;
            }
            send(ServerCommand::MAP_SET, &current_map_idx, sizeof(int), peer);
            send(ServerCommand::ROOM_LIST_BROADCAST, names, sizeof(char) * Structs::NAME_SIZE * Structs::MAX_ROOM_SIZE);
            break;
        }
        case ClientCommand::REMOVE_PLAYER_REQUEST:
        {
            size_t client_id = *(size_t*)message;
            if (members[client_id] != nullptr){
                char reason[Structs::STRING_MESSAGE_SIZE] = "You have been kicked!";
                send(ServerCommand::DISCONNECT, reason, sizeof(char) * Structs::STRING_MESSAGE_SIZE, members[client_id]);
                
                std::cout << "Client(" << client_id << ") has been removed!";
                --current_room_size;
                enet_host_flush(host);
                enet_peer_reset(members[client_id]);
                
                members[client_id] = nullptr;
                strcpy(names[client_id], "");

                // Update the players connacted
                send(ServerCommand::ROOM_LIST_BROADCAST, names, sizeof(char) * Structs::NAME_SIZE * Structs::MAX_ROOM_SIZE);
            }
            break;
        }
        case ClientCommand::START_GAME_REQUEST:
        {
            if (!is_in_game){
                game_state.reset(new GameState());
                game_state.get()->init_state(Structs::MAX_ROOM_SIZE);
            }
                for (size_t i = 0; i < Networking::Message::Room::MAX_ROOM_SIZE; ++i){
                    
                    game_state.get()->old_state[i].is_connected = strcmp(names[i], "") != 0;
                    game_state.get()->old_state[i].is_alive = strcmp(names[i], "") != 0;
                    
                }
                send(ServerCommand::GAME_START);
                is_in_game = true;
            
            // break;
        }
        case ClientCommand::MAP_SET_REQUEST:
        {
            if (!is_in_game){
                current_map_idx = *(int*)message;
                send(ServerCommand::MAP_SET, message, sizeof(int));
            }
            break;
        }
        case ClientCommand::GAME_STATE_UPDATE_REQUEST:
        {
            // if (!is_in_game){break;}
            ++skipped_updates;
            PlayerPacket* pp = (PlayerPacket*)message;
            auto xs = game_state.get()->update_state(pp).data();
            if (skipped_updates > 2){
                skipped_updates = 0;
                send(ServerCommand::GAME_STATE_UPDATE, xs, sizeof(PlayerPacket) * Networking::Message::Room::MAX_ROOM_SIZE);
            }
            break;
        }
        case ClientCommand::REQUEST_SPAWN_DATA:
        {
            // if (!is_in_game) break;
            
            size_t client_id = *(size_t*)message;
            Structs::SpawnData sd;
            sd.map_id = current_map_idx;
            sd.spawn = game_state.get()->game_constants.spawnpoints[client_id];
            sd.angle = 0;
            game_state.get()->old_state[client_id].health = 400;
            game_state.get()->old_state[client_id].is_alive = true;
            game_state.get()->old_state[client_id].is_connected = true;
            game_state.get()->old_state[client_id].score = 0;
            
            
            send(ServerCommand::SET_SPAWN_DATA, &sd, sizeof(Structs::SpawnData), peer);
            
            break;
        }
    
        default:
            break;
    }

    enet_host_flush(host);
}

void RoomHost::on_run()
{
    const size_t host_name_size = 50;
    char host_name[host_name_size];
    ENetAddress sock_addr;
    
    enet_socket_get_address(host->socket, &sock_addr);
    enet_address_get_host_ip(&address, host_name, host_name_size);
    
    std::cout << "Listening on " << host_name << ":" << sock_addr.port << std::endl;
}

void RoomHost::on_stop_request()
{
    std::cout << "Stopping the server" << std::endl;
}

void RoomHost::on_stop()
{
    for (size_t i = 0; i < Networking::Message::Room::MAX_ROOM_SIZE; ++i){
        if (members[i] != nullptr){
            send(Networking::Message::Room::Server::DISCONNECT, members[i]);
            enet_host_flush(host);
            enet_peer_reset(members[i]);
            members[i] = nullptr;
        }
    }
    std::cout << "Stopped the server :)" << std::endl;
}
