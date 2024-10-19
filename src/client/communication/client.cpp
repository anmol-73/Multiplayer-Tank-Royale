#include "client.hpp"

std::pair<bool, std::string> Communication::Client::connect(std::string server_address, const bool &cancel_requested)
{
    destroy_host();
    const size_t outgoing_channel_count = 1;

    auto [ip, port] = Networking::parse_address_string(server_address);
    if (port == -1){
        return {false, std::string("Invalid address | ") + ip};
    }

    host = enet_host_create(
        NULL,   // create a client host
        outgoing_channel_count,
        2,      // allow up 2 channels to be used, 0 and 1
        0,      // assume any amount of incoming bandwidth
        0       // assume any amount of outgoing bandwidth
    );

    if (host == nullptr){
        return {false, "Could not create host!"};
    }

    enet_address_set_host(&address, ip.c_str());
    address.port = port;

    peer = enet_host_connect(host, &address, 2, 0);
    if (peer == nullptr) {
        return {false, "Could not create peer to connect to server!"};
    }

    ENetEvent event;
    const size_t connection_timeout = 500;
    const size_t max_tries = 10;


    for (size_t tries = 0; tries < max_tries; ++tries){
        if (cancel_requested){
            return {false, "Connection request cancelled!"};
        }
        if (enet_host_service(host, &event, connection_timeout) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
        {
            return handle_established_connection(cancel_requested);
        }
        else
        {
            enet_peer_reset(peer);
            return {false, "Could not establish connection to server!"};
        }
    }

    return {false, "Server did not respond in time!"};

}

void Communication::Client::cleanup()
{
    destroy_host();
}

void Communication::Client::run()
{
    const size_t timeout = 500;
    is_running = true;
    on_run();
    ENetEvent event;
    while (is_running){
        if (enet_host_service(host, &event, timeout) <= 0) continue;
        switch (event.type){
            // In the future we must handle the case when disconnects happen (for now assume nobody is malicious)
            case ENET_EVENT_TYPE_RECEIVE:
            {
                auto [type, data] = Networking::decode_message(event.packet->data);
                handle_message(type, data);
                enet_packet_destroy(event.packet);
                break;
            }

            default:
                break;
        }
    }
    on_stop();
}

void Communication::Client::stop()
{
    is_running = false;
}

void Communication::Client::destroy_host()
{
    if (host != nullptr){
        enet_host_destroy(host);
    }
}

std::pair<bool, std::string> Communication::Client::handle_established_connection(const bool& cancel_requested)
{
    return {true, ""};
}

void Communication::Client::handle_message(size_t type, void *message)
{
}

void Communication::Client::on_run()
{
}

void Communication::Client::on_stop()
{
}

void Communication::Client::send(size_t command, void *data, size_t data_size, ENetPacketFlag flag)
{
    auto [encoded_message, encoded_message_size] = Networking::encode_message(command, data, data_size);
    enet_peer_send(
        peer, 0,
        enet_packet_create(encoded_message.get(), encoded_message_size, flag)
    );
}

void Communication::Client::send(size_t command, ENetPacketFlag flag)
{
    enet_peer_send(
        peer, 0,
        enet_packet_create(&command, sizeof(size_t), flag)
    );
}
