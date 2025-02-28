#ifndef H_SERVICE_PROVIDER
#define H_SERVICE_PROVIDER

#include <enet_include.hpp>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <memory>

#include "communication/message_encoding.hpp"

struct ServiceProvider{
public:
    /**
     * NOTE: Setting port to 0 will choose a random port
     */
    ServiceProvider(int port = 0);
    
    /**
     * Starts listening for requests.
     * NOTE: This function blocks until `stop` is called.
     */
    void start();

    /**
     * Stops listening for requests.
     */
    void stop();

    ~ServiceProvider();
    
    ENetAddress address;

protected:
    /**
     * Called before enet_host_service is called
     */
    virtual void update();

    /**
     * Callback when `start` is called
     */
    virtual void on_start();

    /**
     * Callback when the service provider stops listening
     */
    virtual void on_finish();
    
    /**
     * Callback when `stop` is called (i.e when the service provider is asked to stop)
     */
    virtual void on_stop_request();

    /**
     * Callback when a new peer connects to the host
     */
    virtual void handle_new_client(ENetPeer *peer);

    /**
     * Callback when a peer disconnects (either abruptly or non abruptly)
     */
    virtual void handle_disconnection(ENetPeer *peer);

    /**
     * Callback when a message is recieved from a peer
     * NOTE: The memory associated with the message will be freed after the callback completes.
     */
    virtual void handle_message(ENetPeer *peer, Communication::Command type, const void *message, size_t size);

    /**
     * Sends a command (along with the message data) to a specific peer
     */
    void send_message(Communication::Command command, const void* data, size_t data_size, ENetPeer* peer, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
    
    /**
     * Sends a command to a specific peer
     */
    void send_command(Communication::Command command, ENetPeer* peer, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
    
    /**
     * Broadcasts a command (along with some message data) to all connected peers
     */
    void broadcast_message(Communication::Command command, const void* data, size_t data_size, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
    
    /**
     * Broadcasts a command to all connected peers
     */
    void broadcast_command(Communication::Command command, ENetPacketFlag flag = ENetPacketFlag::ENET_PACKET_FLAG_RELIABLE);

    ENetHost *host;

    bool is_running();

private:
    volatile bool _is_running = false;
    
};

#endif