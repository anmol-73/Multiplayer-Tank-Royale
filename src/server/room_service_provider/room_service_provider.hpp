#ifndef H_ROOM_SERVICE_PROVIDER
#define H_ROOM_SERVICE_PROVIDER

#include <vector>
#include <map>
#include <functional>

#include "../game_service_provider/game_service_provider.hpp"
#include "../service_provider.hpp"
#include "communication/protocol.hpp"
#include "misc/task.hpp"


struct RoomServiceProvider: public ServiceProvider{
public:
    RoomServiceProvider(std::function<void(int)> destroy_callback, std::function<void(int, GameServiceProvider*)> register_game, std::function<void(int)> game_destroy_callback, std::string name): ServiceProvider(0), name(name), destroy_callback(destroy_callback), register_game(register_game), game_destroy_callback(game_destroy_callback) {};
    
    std::string name;
    void start_async();

private:
    std::function<void(int)> destroy_callback;
    std::function<void(int, GameServiceProvider*)> register_game;
    std::function<void(int)> game_destroy_callback;
    
    std::map<ENetPeer*, int> peer_to_idx;

    const int max_room_size = 12;
    int room_size = 0; // Keeps track of number of peers connected

    void on_start();
    void on_finish();
    void handle_new_client(ENetPeer *peer);
    void handle_disconnection(ENetPeer *peer);
    void handle_message(ENetPeer *peer, Communication::Command type, const void *message, size_t size);

    Utils::Task life;

    std::vector<Communication::Room::PlayerDetail> get_player_details();

    struct PlayerInfo{
        Communication::Room::PlayerName name;
        ENetPeer* peer;
        int id;
        bool exists;
    };

    bool game_started = false;
    Communication::Room::RoomSettings settings = {};

    std::vector<PlayerInfo> players;
};

#endif