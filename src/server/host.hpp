#ifndef HEADER_HOST
#define HEADER_HOST

#include <iostream>
#if defined(_WIN32)           
	#define NOGDI             // All GDI defines and routines
	#define NOUSER            // All USER defines and routines
#endif
typedef struct tagMSG *LPMSG;
#include <enet/enet.h>

#if defined(_WIN32)           // raylib uses these names as function parameters
	#undef near
	#undef far
    #undef ABSOLUTE
    #undef min
    #undef max
#endif
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
     * Stops the server from running
     */
    void stop();

    /**
     * Cleans up all resources used by the host
     */
    void cleanup();

protected:
    /**
     * Overload this to do some extra logic when the server has started
     */
    virtual void on_run();
    /**
     * Overload this to do some extra logic when the server has finished running
     */
    virtual void on_stop();
    /**
     * Overload this to do some extra logic when the server has asked to finish
     */
    virtual void on_stop_request();

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

    volatile bool is_running = false;
};

#endif