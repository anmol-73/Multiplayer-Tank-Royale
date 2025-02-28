#ifndef H_LOBBY_SERVICE_PROVIDER
#define H_LOBBY_SERVICE_PROVIDER

#include <map>

#include "../service_provider.hpp"
#include "communication/protocol.hpp"
#include "room_service_provider/room_service_provider.hpp"

struct LobbyServiceProvider: public ServiceProvider{
public:
    LobbyServiceProvider(int port = 0): ServiceProvider(port) {};
    
private:
    int connected_count = 0; // Keeps track of number of peers connected (NOTE: Only used for logging)

    std::vector<Communication::Lobby::RoomDetail> get_room_details();

    void on_start();
    void on_finish();
    void update();
    void handle_new_client(ENetPeer *peer);
    void handle_disconnection(ENetPeer *peer);
    void handle_message(ENetPeer *peer, Communication::Command type, const void *message, size_t size);

    std::map<int, std::unique_ptr<RoomServiceProvider>> rooms;
    std::map<int, std::unique_ptr<GameServiceProvider>> games;

    std::vector<int> rooms_to_delete;
    std::vector<int> games_to_delete;
};

#endif