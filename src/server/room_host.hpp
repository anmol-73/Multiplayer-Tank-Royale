#ifndef HEADER_LOBBY_HOST
#define HEADER_LOBBY_HOST

#include <enet/enet.h>
#include <memory>
#include <stdexcept>
#include <map>
#include <vector>
#include "host.hpp"

struct RoomHost: public Host{
    RoomHost();

    bool accept_new_connection();
    void handle_new_client(ENetPeer* peer);
    void handle_disconnection(ENetPeer* peer);
    void handle_message(ENetPeer* peer, size_t type, void* message);

private:
    size_t current_room_size = 0;
    std::vector<std::pair<ENetPeer*, std::string>> members;
};

#endif