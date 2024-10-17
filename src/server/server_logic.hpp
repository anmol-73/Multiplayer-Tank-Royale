#ifndef HEADER_SERVER_LOGIC
#define HEADER_SERVER_LOGIC

#include <raylib.h>
#include <string>
#include <utility>
#include <vector>

struct PlayerPacket
{
    int ID;
    std::string name;
    Vector2 position_absolute;
    float player_angle;
    float gun_angle;
    int health=0;
    int gun_dmg;
    bool has_shot;
    bool is_alive;
    bool is_connected;
};

class GameState
{
    private:
        int max_players=8;

        std::vector<PlayerPacket> old_state;

        void init_state(int max_players);

        void update_state(PlayerPacket received_packet);

        struct GameConstants
        {
            std::vector<Vector2> spawnpoints = {
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
            };

            std::vector<std::pair<int, int>> wall_indices = {
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
                {0, 0},
            };
        } game_constants;
};

#endif
