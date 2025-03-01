#include "room_service_provider.hpp"

#define log(message) std::cout << "[Room Server `" << name << "`] " << message << std::endl

void RoomServiceProvider::start_async()
{
    if (life.is_running()){
        throw std::runtime_error("Service provider was asked to start twice!");
    }
    life.accomplish([this](bool _cancelled){
        start();
    });
}

void RoomServiceProvider::on_start()
{
    const size_t hostname_sz = 16;
    char hostname[hostname_sz];
    enet_address_get_host_ip(&address, hostname, hostname_sz);

    players.assign(max_room_size, {}); {
        for (size_t i = 0; i < players.size(); ++i){
            players[i].id = i;
            players[i].exists = false;
        }
    }

    log("Listening on " << hostname << ':' << address.port);
}

void RoomServiceProvider::on_finish()
{
    log("Stopped servicing :)");
    destroy_callback(address.port);
}

void RoomServiceProvider::handle_new_client(ENetPeer *peer)
{
    const auto default_player_name = "???";
    using namespace Communication::Room;

    // Don't accept connections if the game has started
    if (room_size >= max_room_size or game_started){
        log("Client was not allowed to join");
        send_command(Server::JOIN_DENIED, peer);        
        enet_peer_disconnect(peer, 0);        
        return;
    }

    int id = -1;
    for (auto& player: players){
        if (player.exists) continue;
        
        id = player.id;
        strncpy(player.name, default_player_name, sizeof(PlayerName));
        player.peer = peer;
        peer_to_idx[peer] = id;
        player.exists = true;

        break;
    }

    if (id == -1){
        throw std::runtime_error("Could not assign id even though room not at max capacity!");
    }

    send_message(Server::JOIN_OK, &id, sizeof(int), peer);

    auto pd = get_player_details();
    broadcast_message(Server::PLAYER_LIST, pd.data(), sizeof(PlayerDetail) * pd.size());
    
    log("New client(" << id << ") connected! Current room size: " << ++room_size);
}

void RoomServiceProvider::handle_disconnection(ENetPeer *peer)
{
    using namespace Communication::Room;

    if (peer_to_idx.count(peer) == 0) return;
    
    int id = peer_to_idx[peer];
    peer_to_idx.extract(peer);

    players[id].exists = false;
    players[id].peer = nullptr;

    if (id == 0 and !game_started){
        game_started = true;
        for (const auto& player: players){
            if (player.exists){
                enet_peer_disconnect(player.peer, 0);
            }
        }
    }

    log("Client(" << id << ") disconnected. Current room size: " << --room_size);

    if (room_size <= 0) stop();
}

void RoomServiceProvider::handle_message(ENetPeer *peer, Communication::Command type, const void *message, size_t size)
{
    using namespace Communication::Room;
    if (peer_to_idx.count(peer) == 0){
        log("Unrecognized peer sending message!");
        return;
    }

    const int id = peer_to_idx[peer];

    switch (static_cast<Client>(type)){
        case Client::SET_NAME: {
            auto new_name = static_cast<const char*>(message);
            
            bool is_duplicate = false;
            for (const auto& player: players){
                if (!player.exists) continue;
                if (strncmp(new_name, player.name, sizeof(PlayerName) == 0)){
                    is_duplicate = true;
                    break;
                }
            }

            if (is_duplicate or game_started){
                log("Client(" << id << ") was not allowed to set name to `" << new_name << "`");
                send_command(NAME_DENIED, peer);
                break;
            }
            strncpy(players[id].name, new_name, sizeof(PlayerName));
            auto pd = get_player_details();
            
            log("Client(" << id << ") set name to `" << new_name << "`");
            send_command(NAME_OK, peer);
            broadcast_message(Server::PLAYER_LIST, pd.data(), sizeof(PlayerDetail) * pd.size());

            break;
        }
        case Client::SET_SETTINGS: {
            if (game_started) break;

            log("Updated settings");

            settings = *static_cast<const RoomSettings*>(message);
            broadcast_message(SETTINGS_SET, &settings, sizeof(RoomSettings));

            break;
        }
        case Client::START_GAME: {
            if (game_started) break;
            game_started = true;

            log("Game started");
            auto gsp = new GameServiceProvider(name, game_destroy_callback, get_player_details());
            
            int port = gsp->address.port;
            gsp->start_async(); // TODO! GSPs have to self delete! (Thats very prone to leaking)

            register_game(port, gsp);

            broadcast_message(Server::JOIN_GAME, &port, sizeof(int));

            break;
        }
    }
}

std::vector<Communication::Room::PlayerDetail> RoomServiceProvider::get_player_details()
{
    std::vector<Communication::Room::PlayerDetail> pd; {
        pd.reserve(max_room_size);
        for (const auto& player: players){
            if (!player.exists) continue;
            pd.push_back({});
            pd.back().id = player.id;
            strncpy(pd.back().name, player.name, sizeof(Communication::Room::PlayerName));
        }
    }

    return pd;
}

#undef log
