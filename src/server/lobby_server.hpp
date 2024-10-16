#ifndef HEADER_SERVER
#define HEADER_SERVER

#include <enet/enet.h>
#include <memory>
#include <stdexcept>
#include <map>
#include "../common/lobby_communication.hpp"
#include "../common/common.hpp"

#define MAX_SERVER_PEERS 16
#define MAX_LOBBY_SIZE 8

struct LobbyServer{
public:
    void bind(const char* ip_address, int port);
    void run();
    
    bool handle_connection_request(ENetPeer* event);

    void cleanup();

    size_t generate_client_id();
private:
    ENetAddress address;
    ENetHost* host;

    std::map<size_t, ENetPeer*> clients;
    size_t connected_count = 0;

    size_t __next_id = 0;
};

#endif