#include "server_logic.hpp"
#include <iostream>

void GameState::init_state(int max_players)
{
    GameState::old_state.clear();
    for(int i=0; i<max_players; i++)
    {
        CommonStructs::PlayerPacket packet;
        packet = {
            .is_idle = true,
            .ID=i,
            .position_absolute = GameState::game_constants.spawnpoints[i],
            .player_angle = 0,
            .gun_angle = 0,
            .health = 400,
            .player_dmg = 100,
            .has_shot = false,
            .is_alive = true,
            .is_connected = true,
            .score = 0
        };
        GameState::old_state.push_back(packet);
        GameState::projectiles_vector.push_back({{0,0,0,0},0,0,0,0});
        GameState::start_time = std::chrono::high_resolution_clock().now();
        GameState::time_of_last_proj_update = curtime();
    }
}

std::vector<CommonStructs::PlayerPacket>& GameState::update_state(CommonStructs::PlayerPacket* received_packet)
{
    // std::cout << received_packet->ID << " " << time(nullptr) << std::endl;
    old_state[received_packet->ID].has_shot = received_packet->last_shot != old_state[received_packet->ID].last_shot;
    received_packet->has_shot  = old_state[received_packet->ID].has_shot;
    handle_tank_collision(received_packet);
    if(received_packet->has_shot)   handle_shots(received_packet);
    // std::cout<<received_packet->position_absolute.x<<" "<<received_packet->position_absolute.y<<std::endl;
    old_state[received_packet->ID].gun_angle = received_packet->gun_angle;
    old_state[received_packet->ID].closest_wall_hit = received_packet->closest_wall_hit;
    old_state[received_packet->ID].last_shot = received_packet->last_shot;
    old_state[received_packet->ID].is_idle = received_packet->is_idle;
    return old_state;
};

void GameState::handle_tank_collision(CommonStructs::PlayerPacket* received_packet)
{
    int current_ID = received_packet->ID;
    CommonStructs::PlayerPacket old_packet = GameState::old_state[current_ID];

    // Handle collision detection
    bool player_colliding=false;

    Rectangle collider = {
        .x = received_packet->position_absolute.x,
        .y = received_packet->position_absolute.y,
        .width = GameState::game_constants.player_width,
        .height = GameState::game_constants.player_height,
    };

    // Player wall
    size_t pos_y = (size_t)(received_packet->position_absolute.y/Maps::maps[0].tile_width_units);
    size_t pos_x = (size_t)(received_packet->position_absolute.x/Maps::maps[0].tile_width_units);
    size_t pos_idx = ((Maps::maps[0].map_width_tiles)*pos_y) + pos_x;
    
    for(size_t wall_y = pos_y-150*(Maps::maps[0].map_width_tiles); wall_y<pos_y+150*(Maps::maps[0].map_width_tiles); wall_y++)
    {
        for(size_t wall_x = pos_x-150; wall_x<pos_x+150; wall_x++)
        {
            size_t wall_idx = ((Maps::maps[0].map_width_tiles)*wall_y) + wall_x;
            if((Vector2Distance({(float)wall_x, (float)wall_y}, {(float)pos_x, (float)pos_y})<=150) && Maps::maps[0].walls[wall_idx]==0)
            {
                Rectangle wall = {
                    .x = (float)(wall_x)*(Maps::maps[0].tile_width_units),
                    .y = (float)(wall_y)*(Maps::maps[0].tile_width_units),
                    .width = (Maps::maps[0].tile_width_units),
                    .height = (Maps::maps[0].tile_width_units),
                };
                player_colliding = Physics::sat_collision_detection(wall, 0, collider, received_packet->player_angle);
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
            if (!old_state[i].is_alive) continue;
            if(i!=current_ID)
            {
                Rectangle other_player_collider = {
                    .x = old_state[i].position_absolute.x,
                    .y = old_state[i].position_absolute.y,
                    .width = GameState::game_constants.player_width,
                    .height = GameState::game_constants.player_height,
                };
                player_colliding = Physics::sat_collision_detection(collider, received_packet->player_angle, other_player_collider, old_state[i].player_angle);
                if(player_colliding){break;}
            }
            if(player_colliding){break;}
        }
    }
    if(!player_colliding)
    {
        old_state[current_ID].position_absolute = received_packet->position_absolute;
        old_state[current_ID].player_angle = received_packet->player_angle;
    }
};

// void GameState::handle_shots(CommonStructs::PlayerPacket* player_packet){
//     Vector2 contact_point = player_packet->closest_wall_hit;
//     Vector2 curr_contact_point = contact_point;
//     float min_dis = Vector2Distance(contact_point, player_packet->position_absolute);
//     float cur_dis;
//     bool bullet_colliding;
//     int hitting_idx = -1;
//     if(player_packet->has_shot){
//         std::cout<<"Player"<<player_packet->ID<<" has shot"<<std::endl;
//         for(int i=0; i<12; i++)
//         {   
            
//             if(i!=player_packet->ID && old_state[i].is_alive)
//             {
//                 Rectangle other_player_collider = {
//                     .x = old_state[i].position_absolute.x - 1.0f*(Maps::maps[0].tile_width_units),
//                     .y = old_state[i].position_absolute.y - 0.75f * (Maps::maps[0].tile_width_units),
//                     .width = GameState::game_constants.player_width,
//                     .height = GameState::game_constants.player_height,
//                 };

//                 bullet_colliding = Physics::CheckCollisionRay2dRectEx(player_packet->position_absolute, player_packet->gun_angle, other_player_collider, old_state[i].player_angle, &curr_contact_point);
//                 // bullet_colliding = Physics::CheckCollisionRay2dRect(player_data.position, gun_data.gun_angle, other_player_collider, &curr_contact_point);
//                 cur_dis = Vector2Distance(curr_contact_point, player_packet->position_absolute);
                
//                 if(bullet_colliding){
//                     if(cur_dis < min_dis)
//                     {
//                         min_dis = cur_dis;
//                         contact_point = curr_contact_point;
//                         hitting_idx = i;
//                     }
//                 }
//             }
//         }
//         std::cout<<"Player"<<player_packet->ID<<" shit Player"<<hitting_idx<<std::endl;
//         if(hitting_idx >= 0){
//             if(old_state[hitting_idx].health <= player_packet->player_dmg){
//                 old_state[hitting_idx].is_alive = false;
//                 old_state[hitting_idx].health = 0;
//                 old_state[player_packet->ID].score +=100;
//             }
//             else{
//                 old_state[hitting_idx].health -= player_packet->player_dmg;
//             }
//         }
//     }
//     player_packet->closest_wall_hit = contact_point;
//     player_packet->has_shot = false;
// }

void GameState::handle_shots(CommonStructs::PlayerPacket* player_packet)
{
    CommonStructs::Projectile new_projectile;
    new_projectile.angle = player_packet->gun_angle;
    new_projectile.hitbox =  {
            .x = player_packet->position_absolute.x + (float)(48*cos(player_packet->gun_angle)),
            .y = player_packet->position_absolute.y + (float)(48*sin(player_packet->gun_angle)),
            .width = 12,
            .height = 12,
        };
    new_projectile.time_alive = 0;
    new_projectile.projectile_speed = 200.0f;
    new_projectile.dmg=100;
    new_projectile.shot_id = player_packet->ID;
    projectiles_vector.push_back(new_projectile);
}

std::vector<CommonStructs::Projectile>& GameState::update_projectiles()
{
    float t=(float)curtime();
    float delta_time = (t-time_of_last_proj_update)/1000;
    //std::cout << t << " " << time_of_last_proj_update << " " << delta_time << std::endl; 
    time_of_last_proj_update=t;
    //std::cout << projectiles_vector.size() << std::endl;
    for(int i=1; i<projectiles_vector.size(); i++)
    {
        //std::cout << projectiles_vector[i].hitbox.x << " " << projectiles_vector[i].hitbox.y << " " << projectiles_vector[i].angle << " " << projectiles_vector[i].time_alive << std::endl;
        bool not_colliding = true;

        Rectangle collider = projectiles_vector[i].hitbox;
        size_t pos_y = (size_t)(collider.y/Maps::maps[0].tile_width_units);
        size_t pos_x = (size_t)(collider.x/Maps::maps[0].tile_width_units);
        size_t pos_idx = ((Maps::maps[0].map_width_tiles)*pos_y) + pos_x;
        
        for(size_t wall_y = pos_y-1; wall_y<pos_y+1; wall_y++)
        {
            for(size_t wall_x = pos_x-1; wall_x<pos_x+1; wall_x++)
            {
                size_t wall_idx = ((Maps::maps[0].map_width_tiles)*wall_y) + wall_x;
                if(Maps::maps[0].walls[wall_idx]==0)
                {
                    Rectangle wall = {
                        .x = (float)(wall_x)*(Maps::maps[0].tile_width_units),
                        .y = (float)(wall_y)*(Maps::maps[0].tile_width_units),
                        .width = (Maps::maps[0].tile_width_units),
                        .height = (Maps::maps[0].tile_width_units),
                    };
                    not_colliding = !(Physics::sat_collision_detection(wall, 0, collider, projectiles_vector[i].angle));
                    if(!not_colliding){break;}
                }
            }
            if(!not_colliding){
                std::cout<<"wall"<<" ";
                break;}
        }

        if(not_colliding)
        {
            for(int j=0; j<8; j++)
            {
                if (!old_state[j].is_alive) continue;
                Rectangle other_player_collider = {
                    .x = old_state[j].position_absolute.x,
                    .y = old_state[j].position_absolute.y,
                    .width = GameState::game_constants.player_width,
                    .height = GameState::game_constants.player_height,
                };
                not_colliding = !(Physics::sat_collision_detection(collider, projectiles_vector[i].angle, other_player_collider, old_state[j].player_angle));
                if(!not_colliding){
                    std::cout<<j<<" ";
                    if(old_state[j].health <= projectiles_vector[i].dmg){
                        old_state[j].is_alive = false;
                        old_state[j].health = 0;
                        old_state[projectiles_vector[i].shot_id].score +=100;
                    }
                    else{
                        old_state[j].health -= projectiles_vector[i].dmg;
                    }
                    break;
                }
            }
        }

        if(projectiles_vector[i].time_alive<2 && not_colliding)
        {
            double displacement = (projectiles_vector[i].projectile_speed * delta_time);
            projectiles_vector[i].hitbox.x += (displacement*cos(projectiles_vector[i].angle));
            projectiles_vector[i].hitbox.y += (displacement*sin(projectiles_vector[i].angle));
            projectiles_vector[i].time_alive += delta_time;
        }
        else
        {
            projectiles_vector.erase(projectiles_vector.begin() + i);
        }
    }
    projectiles_vector[0].time_alive = projectiles_vector.size(); // Is the size ;-;
    return projectiles_vector;
}

long double GameState::curtime() {
    auto ctime = std::chrono::high_resolution_clock().now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(ctime-GameState::start_time);
    return duration.count();
}