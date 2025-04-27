#ifndef H_SERVICE_CONSUMER
#define H_SERVICE_CONSUMER

#include <enet_include.hpp>
#include <cassert>
#include <iostream>

#include "communication/message_encoding.hpp"
#include "communication/structs.hpp"

struct ServiceConsumer{
public:
    /**
     * Connects to the address given
     * NOTE: Returns an empty string if succesful (otherwise returns the error message)
     */
    std::string connect(Communication::Address address);

    /**
     * Starts listening for requests.
     * NOTE: This function blocks until `stop` is called.
     */
    void start();

    /**
     * Stops listening for requests.
     */
    void stop();

    virtual ~ServiceConsumer();

    ServiceConsumer(size_t timeout = 500): timeout(timeout){};
    
    ENetAddress address;

protected:
    size_t timeout;    
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
     * Callback when the connection is made
     * Expected to return an empty string if no error (Otherwise return error message)
     */
    virtual std::string on_connection_established();

    /**
     * Callback when the connection is closed
     */
    virtual void handle_disconnection();

    /**
     * Callback when a message is recieved from the server
     * NOTE: The memory associated with the message will be freed after the callback completes.
     */
    virtual void handle_message(Communication::Command type, const void *message, size_t size);

    /**
     * Sends a command (along with the message data) to a specific peer
     */
    void send_message(Communication::Command command, const void* data, size_t data_size, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
    
    /**
     * Sends a command to a specific peer
     */
    void send_command(Communication::Command command, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
    
    
    ENetHost *host = nullptr;
    ENetPeer *peer = nullptr;
    bool is_running();

private:
    volatile bool _is_running = false;
    
};

#endif