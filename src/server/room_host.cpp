#include "room_host.hpp"

RoomHost::RoomHost()
{
    members.assign(Networking::Message::Room::MAX_ROOM_SIZE, {});
}

bool RoomHost::accept_new_connection()
{
    return current_room_size < Networking::Message::Room::MAX_ROOM_SIZE;
}

void RoomHost::handle_new_client(ENetPeer *peer)
{
    ++current_room_size;
    for (size_t i = 0; i < members.size(); ++i){
        if (members[i] == nullptr){
            members[i] = peer;
            send(Networking::Message::Room::Server::ASSIGN_ID, &i, sizeof(size_t), peer);
            return;
        }
    }
    throw std::runtime_error("Critical logic error! Could not assign ID to room member");
}

void RoomHost::handle_disconnection(ENetPeer *peer)
{
    for (size_t i = 0; i < members.size(); ++i){
        // Handle the case where the client didn't properly disconnect also :)
        if (members[i] == peer){
            enet_peer_reset(peer);
            members[i] = nullptr;
            break;
        }
    }
}

void RoomHost::handle_message(ENetPeer *peer, size_t type, void *message)
{
    namespace Structs = Networking::Message::Room;
    using ClientCommand = Structs::Client;
    using ServerCommand = Structs::Server;
    switch (type)
    {
        case ClientCommand::NAME_SET_REQUEST:
        {
            Structs::NameSetRequest *name = (Structs::NameSetRequest*)message;
            strncpy(names[name->client_id], name->name, Structs::NAME_SIZE);
            send(ServerCommand::ROOM_LIST_BROADCAST, name, sizeof(char) * Structs::NAME_SIZE * Structs::MAX_ROOM_SIZE);
            break;
        }
        case ClientCommand::REMOVE_PLAYER_REQUEST:
        {
            size_t client_id = *(size_t*)message;
            if (members[client_id] != nullptr){
                send(ServerCommand::DISCONNECT, members[client_id]);
                enet_peer_reset(members[client_id]);
                members[client_id] = nullptr;            
            }
            break;
        }
        case ClientCommand::START_GAME_REQUEST:
        {
            send(ServerCommand::GAME_START);
            break;
        }
    
        default:
            break;
    }
}

void RoomHost::on_run()
{
    const size_t host_name_size = 50;
    char host_name[host_name_size];
    enet_address_get_host_ip(&address, host_name, host_name_size);
    std::cout << "Listening on " << host_name << std::endl;
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
            enet_peer_reset(members[i]);
            members[i] = nullptr;
        }
    }
    std::cout << "Stopped the server :)" << std::endl;
}
