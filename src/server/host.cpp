#include "host.hpp"

void Host::bind(int port)
{
    address.host = ENET_HOST_ANY;
    address.port = port;

    host = enet_host_create(
        &address,           // the address to bind the server host to 
        MAX_HOST_PEERS,     // allow up to 16 clients and/or outgoing connections
        2,                  // allow up to 2 channels to be used, 0 and 1
        0,                  // assume any amount of incoming bandwidth
        0                   // assume any amount of outgoing bandwidth
    );

    if (host == nullptr){
        throw std::runtime_error("Could not create Enet server host!");
    }
}

void Host::run()
{
    on_run();
    const size_t timeout = 3000;
    is_running = true;
    
    ENetEvent event;
    while (is_running){
        enet_host_service(host, &event, timeout);
        switch (event.type){
            case ENET_EVENT_TYPE_CONNECT:
            {
                bool accepted_new_connection = accept_new_connection();
                size_t response = accepted_new_connection? Networking::Message::ConnectStatus::OK : Networking::Message::ConnectStatus::DENIED;
                
                send(response, event.peer);
                
                if (accepted_new_connection){
                    handle_new_client(event.peer);
                } else{
                    enet_peer_reset(event.peer);
                }
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                handle_disconnection(event.peer);
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                auto [type, data] = Networking::decode_message(event.packet->data);
                handle_message(event.peer, type, data);
                break;
            }

            case ENET_EVENT_TYPE_NONE:
                break;
        }
    }
    on_stop();
}

void Host::stop()
{
    if (!is_running) return;
    is_running = false;
    on_stop_request();
}

void Host::cleanup()
{
    enet_host_destroy(host);
}


bool Host::accept_new_connection()
{
    return true;
}

void Host::on_run(){}
void Host::on_stop_request(){}
void Host::on_stop(){}
void Host::handle_new_client(ENetPeer *peer) {}
void Host::handle_disconnection(ENetPeer *peer){}
void Host::handle_message(ENetPeer *peer, size_t type, void *message){}

void Host::send(size_t command, void *data, size_t data_size, ENetPeer *peer, ENetPacketFlag flag)
{
    auto [encoded_message, encoded_message_size] = Networking::encode_message(command, data, data_size);
    enet_peer_send(
        peer, 0,
        enet_packet_create(encoded_message.get(), encoded_message_size, flag)
    );
}

void Host::send(size_t command, ENetPeer *peer, ENetPacketFlag flag)
{
    enet_peer_send(
        peer, 0,
        enet_packet_create(&command, sizeof(size_t), flag)
    );
}

void Host::send(size_t command, void *data, size_t data_size, ENetPacketFlag flag)
{
    auto [encoded_message, encoded_message_size] = Networking::encode_message(command, data, data_size);
    enet_host_broadcast(
        host, 0,
        enet_packet_create(encoded_message.get(), encoded_message_size, flag)
    );
}

void Host::send(size_t command, ENetPacketFlag flag)
{
    enet_host_broadcast(
        host, 0,
        enet_packet_create(&command, sizeof(size_t), flag)
    );
}
