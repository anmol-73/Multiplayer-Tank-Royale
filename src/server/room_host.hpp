#ifndef HEADER_ROOM_HOST
#define HEADER_ROOM_HOST

#include <enet/enet.h>
#include <memory>
#include <stdexcept>
#include <map>
#include <vector>
#include "host.hpp"

struct RoomHost: public Host{
    RoomHost();

    void handle_new_client(ENetPeer* peer);
    void handle_disconnection(ENetPeer* peer);
    void handle_message(ENetPeer* peer, size_t type, void* message);
    
    void on_stop_request();
    void on_stop();
    void on_run();

private:
    size_t current_room_size = 0;
    std::vector<ENetPeer*> members;
    char names[Networking::Message::Room::MAX_ROOM_SIZE][Networking::Message::Room::NAME_SIZE];
};

#endif