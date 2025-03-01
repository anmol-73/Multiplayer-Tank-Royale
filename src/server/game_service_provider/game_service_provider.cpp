#include "game_service_provider.hpp"

#define log(message) std::cout << "[Game Server] " << message << std::endl


void GameServiceProvider::start_async()
{
    if (life.is_running()){
        throw std::runtime_error("Service provider was asked to start twice!");
    }
    life.accomplish([this](bool _cancelled){
        start();
    });
}

void GameServiceProvider::on_start()
{
    const size_t hostname_sz = 16;
    char hostname[hostname_sz];
    enet_address_get_host_ip(&address, hostname, hostname_sz);
    peers.assign(12, nullptr);

    log("Listening on " << hostname << ':' << address.port);
}

void GameServiceProvider::on_finish()
{
    log("Stopped servicing :(");
}

void GameServiceProvider::handle_new_client(ENetPeer *peer)
{
}

void GameServiceProvider::handle_disconnection(ENetPeer *peer)
{
    log("Client disconnected.");
}

void GameServiceProvider::handle_message(ENetPeer *peer, Communication::Command type, const void *message, size_t size)
{
    using namespace Communication::Game;

    switch (static_cast<Client>(type)){
        case Client::IDENTIFY: {
            PlayerIdentification pi = *static_cast<const PlayerIdentification*>(message);
            if(game_already_started || game_state.player_vector[pi.id].exists) break;
            peers[pi.id] = peer;
            game_state.player_vector[pi.id].exists = true;
            num_of_players_joined++;
            if(num_of_players_joined==players_list.size())
            {
                start_game();
            }
            break;
        }
        case Client::FRAME:
            game_state.apply_frame(*static_cast<const Game::Frame*>(message));
            break;
        case Client::TYPES_SELECTION: {

            Game::TypeSelection selection = *static_cast<const Game::TypeSelection*>(message);
            game_state.player_vector[selection.player_id].gun_type = selection.gun_type;
            game_state.player_vector[selection.player_id].tank_type = selection.tank_type;
            game_state.player_vector[selection.player_id].is_alive = true;
            game_state.player_vector[selection.player_id].health = Game::Data::tank_types[selection.player_id].max_health;
            game_state.player_vector[selection.player_id].angle = 0;
            game_state.player_vector[selection.player_id].position = Maps::maps[0].spawnpoints[selection.player_id];
            dead_times[selection.player_id] = 0;
            break;
        }
    }
}

void GameServiceProvider::start_game()
{
    if(game_already_started) return;
    game_state.init_game_state();
    dead_times.assign(12, 0);
    broadcast_command(Communication::Game::Server::RESPAWN_OK);
    timed_loop.accomplish([this](bool _cancelled){
        timed_loop_func();
    });
    game_already_started = true;
}

void GameServiceProvider::timed_loop_func()
{
    time_at_last_broadcast = game_state.curtime();
    while(is_running())
    {
        double t = game_state.curtime();
        double delta_time = t - time_at_last_broadcast;
        std::this_thread::sleep_for(std::chrono::seconds(1/60));
        if((delta_time)>(1/60))
        {
            time_at_last_broadcast = t;
            game_state.update_projectiles(delta_time);
            game_state.update_explosions(delta_time);
            auto [message, sz] = serialize_game_state(game_state); // TODO: make
            broadcast_message(Communication::Game::Server::GAME_STATE_BROADCAST, message, sz);
        }
        for(size_t i=0; i<12; i++)
        {
            if(!game_state.player_vector[i].is_alive)
            {
                dead_times[i]+=delta_time;
                if(dead_times[i]>3000)
                {
                    send_command(Communication::Game::Server::RESPAWN_OK, peers[i]);
                }
            }
        }
    }
}

#undef log