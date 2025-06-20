#include "game_service_provider.hpp"

#define log(message) std::cout << "[Game Server `" << name << "`] " << message << std::endl


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
    game_state.init_game_state(settings.map);
    dead_times.assign(12, 0);
    respawn_ok_sent.assign(12, true);

    log("Listening on " << hostname << ':' << address.port);
    if (players_list.size() == 0){
        log("ERROR! Number of players expected is 0");
        stop();
    }
}

void GameServiceProvider::on_finish()
{
    log("Stopped servicing :(");
    destroy_callback(address.port);
}

void GameServiceProvider::handle_new_client(ENetPeer *peer)
{
    if (game_already_started){
        enet_peer_disconnect(peer, 0);
        log("New client rejected!");
        return;
    }
    log("New unidentified client joined :)");
}

void GameServiceProvider::handle_disconnection(ENetPeer *peer)
{
    for (size_t i = 0; i < peers.size(); ++i){
        if (peers[i] == peer){
            peers[i] = nullptr;
            game_state.player_vector[i].exists = false;
        }

        --num_of_players_joined;
    }
    log("Client disconnected.");

    if (game_over and num_of_players_joined <= 0){
        stop();
    }
}

void GameServiceProvider::handle_message(ENetPeer *peer, Communication::Command type, const void *message, size_t size)
{
    using namespace Communication::Game;
    if (game_over or !is_running()) return;
    std::unique_lock<std::mutex> lk(gs_mutex);
    switch (static_cast<Client>(type)){
        case Client::IDENTIFY: {
            
            PlayerIdentification pi = *static_cast<const PlayerIdentification*>(message);
            
            if(game_already_started || game_state.player_vector[pi.id].exists) break;
            peers[pi.id] = peer;
            game_state.player_vector[pi.id].exists = true;
            
            log("Player with ID(" << pi.id << ") has joined :)" << players_list.size());
            num_of_players_joined++;
            if(num_of_players_joined == static_cast<int>(players_list.size()))
            {
                start_game();
            }
            auto [message, sz] = serialize_game_state(game_state);
            
            broadcast_message(Communication::Game::Server::GAME_STATE_BROADCAST, message.get(), sz);
            break;
        }

        case Client::FRAME: {
            // auto t1 = game_state.curtime();
            game_state.apply_frame(*static_cast<const Game::Frame*>(message));
            // auto t2 = game_state.curtime();
            // log("APPLY " << t2 - t1);
            break;
        }

        case Client::TYPES_SELECTION: {
            
            
            Game::TypeSelection selection = *static_cast<const Game::TypeSelection*>(message);
    
            log("Player(" << selection.player_id << ") spawned in!");
            
            game_state.player_vector[selection.player_id].gun_type = selection.gun_type;
            game_state.player_vector[selection.player_id].tank_type = selection.tank_type;
            game_state.player_vector[selection.player_id].is_alive = true;
            game_state.player_vector[selection.player_id].health = Game::Data::tank_types[selection.tank_type].max_health;
            game_state.player_vector[selection.player_id].angle = 0;
            game_state.player_vector[selection.player_id].position = Maps::maps[settings.map].spawnpoints[selection.player_id];
            dead_times[selection.player_id] = 0;
            respawn_ok_sent[selection.player_id] = false;
            break;
        }
    }
}

void GameServiceProvider::start_game()
{
    if(game_already_started) return;
    game_already_started = true;
    
    broadcast_command(Communication::Game::Server::RESPAWN_OK);
    
    timed_loop.accomplish([this](bool _cancelled){
        timed_loop_func();
    });
    log("Started game!");
}

void GameServiceProvider::timed_loop_func()
{
    time_at_last_broadcast = game_state.curtime();
    while(is_running())
    {
        // 60 fps
        double t = game_state.curtime();
        if (t > 240000){ // Game timer
            game_over = true;
            log("Game over :)");
            broadcast_command(Communication::Game::Server::GAME_OVER);
            if (num_of_players_joined <= 0) stop();
            break;
        }
        
        double delta_time = t - time_at_last_broadcast;
        time_at_last_broadcast = t;

        {
            std::unique_lock<std::mutex> lk(gs_mutex);
            game_state.update_projectiles(delta_time);
            game_state.update_explosions(delta_time);
            auto [message, sz] = serialize_game_state(game_state);
            
            broadcast_message(Communication::Game::Server::GAME_STATE_BROADCAST, message.get(), sz);
            enet_host_flush(host);
            
            for(size_t i=0; i < game_state.player_vector.size(); ++i)
            {
                if((!game_state.player_vector[i].is_alive) and (!respawn_ok_sent[i]))
                {
                    dead_times[i] += delta_time;
                    if(dead_times[i] > 3000)
                    {
                        send_command(Communication::Game::Server::RESPAWN_OK, peers[i]);
                        respawn_ok_sent[i] = true;
                    }
                }
            }
        }
        
        // double t2 = game_state.curtime();
        // log(t2 - t);
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }
}

#undef log