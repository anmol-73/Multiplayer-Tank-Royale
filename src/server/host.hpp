#ifndef HEADER_HOST
#define HEADER_HOST

#include <enet/enet.h>
#include <memory>
#include <stdexcept>
#include <map>
#include "networking.hpp"

#define MAX_HOST_PEERS 16

struct Host{
public:
    /**
     * Binds the socket to a specific port
     * NOTE: Give 0 to bind to any free port
     */
    void bind(int port = 0);
    
    /**
     * Starts listening and handling connections
     */
    void run();

    /**
     * Cleans up all resources used by the host
     */
    void cleanup();

protected:
    /**
     * Overload this to specify whether to accept a new connection or not
     */
    virtual bool accept_new_connection();

    /**
     * Overload this to handle what logic to do when a client connects
     */
    virtual void handle_new_client(ENetPeer* peer);

    /**
     * Overload this to do some logic when a client disconnects
     */
    virtual void handle_disconnection(ENetPeer* peer);

    /**
     * Overload this to do some logic when the client sends a message
     * NOTE: This provides the decoded message (Refer to `common/networking/helper.hpp`)
     */
    virtual void handle_message(ENetPeer* peer, size_t type, void* message);

    /**
     * Sends a command (along with the message data) to a specific peer
     */
    void send(size_t command, void* data, size_t data_size, ENetPeer* peer, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
    
    /**
     * Sends a command to a specific peer
     */
    void send(size_t command, ENetPeer* peer, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
    
    /**
     * Broadcasts a command (along with some message data) to all connected peers
     */
    void send(size_t command, void* data, size_t data_size, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
    
    /**
     * Broadcasts a command to all connected peers
     */
    void send(size_t command, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);

    ENetAddress address;
    ENetHost* host;

private:

    bool is_running = false;
};

#endif