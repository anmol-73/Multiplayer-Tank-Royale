#ifndef H_GAME_SERVICE_PROVIDER
#define H_GAME_SERVICE_PROVIDER

#include "game.hpp"
#include "../service_provider.hpp"
#include "communication/protocol.hpp"
#include "misc/task.hpp"
#include "game_communication.hpp"
#include "thread"

struct GameServiceProvider: public ServiceProvider{
public:
    GameServiceProvider(std::string name, std::function<void(int)> destroy_callback, std::vector<Communication::Room::PlayerDetail> players_list): ServiceProvider(0), name(name), destroy_callback(destroy_callback), players_list(players_list) {};
    void start_async();
    std::string name;  
    std::function<void(int)> destroy_callback;  
private:
    
    void on_start();
    void on_finish();
    void handle_new_client(ENetPeer *peer);
    void handle_disconnection(ENetPeer *peer);
    void handle_message(ENetPeer *peer, Communication::Command type, const void *message, size_t size);

    void start_game();

    void timed_loop_func();

    Game::GameState game_state;

    std::vector<Communication::Room::PlayerDetail> players_list;

    std::vector<ENetPeer *> peers;

    size_t num_of_players_joined;

    std::vector<double> dead_times;

    bool game_already_started = false;

    double time_at_last_broadcast;

    Utils::Task timed_loop;

    Utils::Task life;
};

#endif