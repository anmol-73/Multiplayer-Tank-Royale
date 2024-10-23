#include "server_logic.hpp"

void GameState::init_state(int max_players)
{
    for(int i=0; i<max_players; i++)
    {
        PlayerPacket packet;
        packet = {
            .ID=i,
            .position_absolute_units = GameState::game_constants.spawnpoints[i],
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
    handle_tank_collision(received_packet);
};

void GameState::handle_tank_collision(PlayerPacket received_packet)
{
    int current_ID = received_packet.ID;
    PlayerPacket old_packet = GameState::old_state[current_ID];

    // Handle collision detection
    bool player_colliding=false;

    Rectangle collider = {
        .x = received_packet.position_absolute_units.x,
        .y = received_packet.position_absolute_units.y,
        .width = GameState::game_constants.player_width,
        .height = GameState::game_constants.player_height,
    };

    // Player wall
    size_t pos_y = (size_t)(received_packet.position_absolute_units.y/GameState::game_constants.wall_width);
    size_t pos_x = (size_t)(received_packet.position_absolute_units.x/GameState::game_constants.wall_width);
    size_t pos_idx = ((Maps::map1.map_width_tiles)*pos_y) + pos_x;
    
    for(size_t wall_y = pos_y-150*(Maps::map1.map_width_tiles); wall_y<pos_y+150*(Maps::map1.map_width_tiles); wall_y++)
    {
        for(size_t wall_x = pos_x-150; wall_x<pos_x+150; wall_x++)
        {
            size_t wall_idx = ((Maps::map1.map_width_tiles)*wall_y) + wall_x;
            if((Vector2Distance({(float)wall_x, (float)wall_y}, {(float)pos_x, (float)pos_y})<=150) && Maps::map1.walls[wall_idx]==0)
            {
                Rectangle wall = {
                    .x = (float)(wall_x)*(GameState::game_constants.wall_width),
                    .y = (float)(wall_y)*(GameState::game_constants.wall_width),
                    .width = (GameState::game_constants.wall_width),
                    .height = (GameState::game_constants.wall_width),
                };
                player_colliding = Physics::sat_collision_detection(wall, 0, collider, received_packet.player_angle);
                if(player_colliding){break;}
            }
            if(player_colliding){break;}
        }
        if(player_colliding){break;}
    }
    if(!player_colliding)
    {
        // Player player
        for(int i=0; i<8; i++)
        {
            if(i!=current_ID)
            {
                Rectangle other_player_collider = {
                    .x = old_state[i].position_absolute_units.x,
                    .y = old_state[i].position_absolute_units.y,
                    .width = GameState::game_constants.player_width,
                    .height = GameState::game_constants.player_height,
                };
                player_colliding = Physics::sat_collision_detection(collider, received_packet.player_angle, other_player_collider, old_state[i].player_angle);
                if(player_colliding){break;}
            }
            if(player_colliding){break;}
        }
    }
    if(!player_colliding)
    {
        old_state[current_ID].position_absolute_units = received_packet.position_absolute_units;
        old_state[current_ID].player_angle = received_packet.player_angle;
    }
};
