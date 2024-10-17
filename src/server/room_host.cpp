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
        if (members[i].first == nullptr){
            send(Networking::Message::Room::Server::ASSIGN_ID, &i, sizeof(size_t), peer);
            members[i] = {peer, "New player"};
            return;
        }
    }
    throw std::runtime_error("Critical logic error! Could not assign ID to room member");
}

void RoomHost::handle_disconnection(ENetPeer *peer)
{
    for (size_t i = 0; i < members.size(); ++i){
        // Handle the case where the client didn't properly disconnect also :)
        if (members[i].first == peer){
            members[i].first = nullptr;
            members[i].second.clear();
        }
    }
}

void RoomHost::handle_message(ENetPeer *peer, size_t type, void *message)
{
    using ClientCommand = Networking::Message::Room::Client;
    using ServerCommand = Networking::Message::Room::Server;
    switch (type)
    {
    case ClientCommand::NAME_SET_REQUEST:
        
        break;
    case ClientCommand::REMOVE_PLAYER_REQUEST:
        
        break;
    case ClientCommand::START_GAME_REQUEST:
        
        break;
    
    default:
        break;
    }
}
