#ifndef HEADER_ROOM_HOST
#define HEADER_ROOM_HOST
#include <raylib.h>
#if defined(_WIN32)           
	#define NOGDI             // All GDI defines and routines
	#define NOUSER            // All USER defines and routines
#endif
typedef struct tagMSG *LPMSG;
#include <enet/enet.h>

#if defined(_WIN32)           // raylib uses these names as function parameters
	#undef near
	#undef far
    #undef ABSOLUTE
    #undef min
    #undef max
#endif
#include <memory>
#include <stdexcept>
#include <map>
#include <vector>
#include "host.hpp"

#include "server_logic.hpp"

struct RoomHost: public Host{
    RoomHost();

    void handle_new_client(ENetPeer* peer);
    void handle_disconnection(ENetPeer* peer);
    void handle_message(ENetPeer* peer, size_t type, void* message);
    
    void on_stop_request();
    void on_stop();
    void on_run();

    bool is_in_game = false;


private:
    int skipped_updates = 0;
    std::unique_ptr<GameState> game_state;
    size_t current_room_size = 0;
    int current_map_idx = 0;
    std::vector<ENetPeer*> members;
    char names[Networking::Message::Room::MAX_ROOM_SIZE][Networking::Message::Room::NAME_SIZE];
};

#endif