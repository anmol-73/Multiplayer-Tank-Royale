#include "service_consumer.hpp"


std::string ServiceConsumer::connect(Communication::Address _address)
{
    assert(!_address.is_invalid());
    
    host = enet_host_create(
        NULL,   // create a client host
        1,
        2,      // allow up 2 channels to be used, 0 and 1
        0,      // assume any amount of incoming bandwidth
        0       // assume any amount of outgoing bandwidth
    );


    if (host == nullptr){
        return "Could not create host!";
    }

    enet_address_set_host(&address, _address.name.c_str());
    address.port = _address.port;

    peer = enet_host_connect(host, &address, 2, 0);
    if (peer == nullptr) {
        return "Could not create peer to connect to server!";
    }

    ENetEvent event;
    const size_t connection_timeout = 500;
    const size_t max_tries = 10;

    for (size_t tries = 0; tries < max_tries; ++tries){
        if (enet_host_service(host, &event, connection_timeout) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
        {
            return on_connection_established();
        }
    }

    return "Could not establish connection to server!";
}

void ServiceConsumer::start()
{
    on_start();

    _is_running = true;

    ENetEvent event;
    while (is_running()){
        if (enet_host_service(host, &event, timeout) <= 0) continue;
        switch (event.type){
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                handle_disconnection(); 
                enet_peer_reset(peer);  
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                assert(event.packet->dataLength >= sizeof(Communication::Command));
                
                auto [type, data] = Communication::decode_message(event.packet->data);
                handle_message(type, data, event.packet->dataLength - sizeof(Communication::Command));
                
                enet_packet_destroy(event.packet);
                break;
            }

            case ENET_EVENT_TYPE_NONE:{
                break;
            }

            case ENET_EVENT_TYPE_CONNECT:{
                break;
            }
        }
    }
    
    enet_host_flush(host);
    on_finish();
}

void ServiceConsumer::stop()
{
    if (!is_running()) return;
    
    on_stop_request();
    _is_running = false;
}

ServiceConsumer::~ServiceConsumer()
{
    if (host == nullptr) return;
    enet_host_destroy(host);
    host = nullptr;
}

void ServiceConsumer::on_start(){}

void ServiceConsumer::on_finish(){}

void ServiceConsumer::on_stop_request(){}

std::string ServiceConsumer::on_connection_established()
{
    return std::string();
}

void ServiceConsumer::handle_disconnection(){
    stop();
}

void ServiceConsumer::handle_message(Communication::Command type, const void *message, size_t size){}

void ServiceConsumer::send_message(Communication::Command command, const void *data, size_t data_size, ENetPacketFlag flag)
{
    auto [encoded_message, encoded_message_size] = Communication::encode_message(command, data, data_size);
    enet_peer_send(
        peer, 0,
        enet_packet_create(encoded_message.get(), encoded_message_size, flag)
    );
}

void ServiceConsumer::send_command(Communication::Command command, ENetPacketFlag flag)
{
    if (!is_running()){ // TODO: Throwing now just for sanity check. Remove this before rc
        throw std::runtime_error("Tried to send command after disconnection!");
    }
    enet_peer_send(
        peer, 0,
        enet_packet_create(&command, sizeof(Communication::Command), flag)
    );
}

bool ServiceConsumer::is_running()
{
    return _is_running;
}
