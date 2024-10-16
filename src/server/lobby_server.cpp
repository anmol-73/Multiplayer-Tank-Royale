#include "lobby_server.hpp"

void LobbyServer::bind(const char *ip_address, int port)
{
    enet_address_set_host(&address, ip_address);
    address.port = port;

    host = enet_host_create(
        &address,           // the address to bind the server host to 
        MAX_SERVER_PEERS,   // allow up to 16 clients and/or outgoing connections
        2,                  // allow up to 2 channels to be used, 0 and 1
        0,                  // assume any amount of incoming bandwidth
        0                   // assume any amount of outgoing bandwidth
    );

    if (host == nullptr){
        throw std::runtime_error("Could not create Enet server host!");
    }
}

void LobbyServer::run()
{
    while (true){
        ENetEvent event;

        while (enet_host_service(host, &event, 0) > 0){
            switch (event.type){
            case ENET_EVENT_TYPE_CONNECT:
                handle_connection_request(event.peer);
                break;
            }
        }
    }
}

bool LobbyServer::handle_connection_request(ENetPeer* peer)
{
    Networking::Lobby::JoinResponse response;
    
    if (connected_count >= MAX_LOBBY_SIZE){
        response.status = Networking::ResponseStatus::CONNECT_DENIED;
        strcpy(response.message, "Lobby is full!");

        auto [encoded_message, encoded_message_size] = Networking::encode_message(Networking::Lobby::ResponseType::JOIN_RESPONSE, (void*)&response, sizeof(Networking::Lobby::JoinResponse));
        enet_peer_send(
            peer, 0,
            enet_packet_create(encoded_message.get(), encoded_message_size, ENET_PACKET_FLAG_RELIABLE)
        );
        enet_peer_reset(peer);
        return false;
    }
    response.status = Networking::ResponseStatus::CONNECT_OK;
    response.client_id = generate_client_id();
    auto [encoded_message, encoded_message_size] = Networking::encode_message(Networking::Lobby::ResponseType::JOIN_RESPONSE, (void*)&response, sizeof(Networking::Lobby::JoinResponse));
    enet_peer_send(
        peer, 0,
        enet_packet_create(encoded_message.get(), encoded_message_size, ENET_PACKET_FLAG_RELIABLE)
    );
    clients[response.client_id] = peer;

    return true;
}

void LobbyServer::cleanup()
{
    enet_host_destroy(host);
}

size_t LobbyServer::generate_client_id()
{
    return __next_id++;
}
