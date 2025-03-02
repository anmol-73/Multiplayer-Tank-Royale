#include "service_provider.hpp"

const size_t max_host_peers = 16;
const size_t timeout = 20;

ServiceProvider::ServiceProvider(int port){
    address.host = ENET_HOST_ANY;
    address.port = port;

    host = enet_host_create(
        &address,           // the address to bind the server host to 
        max_host_peers,     // allow up to 16 clients and/or outgoing connections
        2,                  // allow up to 2 channels to be used, 0 and 1
        0,                  // assume any amount of incoming bandwidth
        0                   // assume any amount of outgoing bandwidth
    );

    if (host == nullptr){
        throw std::runtime_error("Could not create Enet server host!");
    }

    enet_socket_get_address(host->socket, &address);
}

void ServiceProvider::start()
{
    on_start();

    _is_running = true;

    ENetEvent event;
    while (is_running()){
        update();
        if (enet_host_service(host, &event, timeout) <= 0) continue;
        switch (event.type){
            case ENET_EVENT_TYPE_CONNECT:
            {
                handle_new_client(event.peer);
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                handle_disconnection(event.peer); 
                enet_peer_reset(event.peer);  
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                assert(event.packet->dataLength >= sizeof(Communication::Command));
                
                auto [type, data] = Communication::decode_message(event.packet->data);
                handle_message(event.peer, type, data, event.packet->dataLength - sizeof(Communication::Command));
                
                enet_packet_destroy(event.packet);
                break;
            }

            case ENET_EVENT_TYPE_NONE:{
                break;
            }
        }
    }
    enet_host_flush(host);

    on_finish();
}

void ServiceProvider::stop()
{
    if (!is_running()) return;
    
    on_stop_request();
    _is_running = false;
}

ServiceProvider::~ServiceProvider()
{
    enet_host_destroy(host);
}

void ServiceProvider::update(){}

void ServiceProvider::on_start() {}

void ServiceProvider::on_finish(){}

void ServiceProvider::on_stop_request(){}

void ServiceProvider::handle_new_client(ENetPeer *peer){}

void ServiceProvider::handle_disconnection(ENetPeer *peer){}

void ServiceProvider::handle_message(ENetPeer *peer, Communication::Command type, const void *message, size_t size){}

void ServiceProvider::send_message(Communication::Command command, const void *data, size_t data_size, ENetPeer *peer, ENetPacketFlag flag)
{
    auto [encoded_message, encoded_message_size] = Communication::encode_message(command, data, data_size);
    enet_peer_send(
        peer, 0,
        enet_packet_create(encoded_message.get(), encoded_message_size, flag)
    );
}

void ServiceProvider::send_command(Communication::Command command, ENetPeer *peer, ENetPacketFlag flag)
{
    enet_peer_send(
        peer, 0,
        enet_packet_create(&command, sizeof(Communication::Command), flag)
    );
}

void ServiceProvider::broadcast_message(Communication::Command command, const void *data, size_t data_size, ENetPacketFlag flag)
{
    auto [encoded_message, encoded_message_size] = Communication::encode_message(command, data, data_size);
    enet_host_broadcast(
        host, 0,
        enet_packet_create(encoded_message.get(), encoded_message_size, flag)
    );
}

void ServiceProvider::broadcast_command(Communication::Command command, ENetPacketFlag flag)
{
    enet_host_broadcast(
        host, 0,
        enet_packet_create(&command, sizeof(Communication::Command), flag)
    );
}

bool ServiceProvider::is_running()
{
    return _is_running;
}
