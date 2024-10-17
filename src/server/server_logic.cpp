#include "server_logic.hpp"

void GameState::init_state(int max_players)
{
    for(int i=0; i<max_players; i++)
    {
        PlayerPacket packet;
        packet = {
            .ID=i,
            .name = "",
            .position_absolute = GameState::game_constants.spawnpoints[i],
            .player_angle = 0,
            .gun_angle = 0,
            .health = 5,
            .gun_dmg = 1,
            .has_shot = false,
            .is_alive = false,
            .is_connected = false,
        };
        GameState::old_state.push_back(packet);
    }
};

void GameState::update_state(PlayerPacket received_packet)
{
    int current_ID = received_packet.ID;
    PlayerPacket old_packet = GameState::old_state[current_ID];

};
