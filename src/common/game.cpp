#include "game.hpp"
#include <iostream>

void Game::GameState::init_game_state(size_t map)
{
    map_num = map;
    num_of_players = 0;
    for(size_t i=0; i<12; i++)
    {
        Game::GameState::PlayerState empty_player;
        empty_player.exists = false;
        empty_player.is_alive = false;
        empty_player.angle = 0;
        empty_player.gun_angle = 0;
        empty_player.position = Maps::maps[map_num].spawnpoints[i];
        empty_player.score = 0;
        empty_player.time_since_last_shot = 0;
        empty_player.last_frame_processed_num = 0;
        empty_player.ray_contact = empty_player.position;
        player_vector.push_back(empty_player);
    }
    
    Game::GameState::start_time = std::chrono::high_resolution_clock().now();
}

void Game::GameState::apply_frame(const Game::Frame &frame)
{
    
    curr_frame = frame;
    player_vector[curr_frame.player_id].last_frame_processed_num = curr_frame.frame_num;
    
    if(!player_vector[curr_frame.player_id].is_alive) return;
    
    handle_movement();
    set_gun_angle();
    handle_shots();
    float t=static_cast<float>(curtime());
    time_of_last_objects_update=t;
    return;
}

double Game::GameState::normalize_angle(double angle)
{
    while (angle >= 2 * PI) angle -= 2.0 * PI;
    while (angle < 0) angle += 2.0 * PI;
    return angle;
}

void Game::GameState::handle_movement()
{
    size_t ID = curr_frame.player_id;
    double projected_angle = player_vector[ID].angle;
    double dx=0;
    double dy=0;
    if(curr_frame.a_pressed || curr_frame.d_pressed || curr_frame.s_pressed || curr_frame.w_pressed)
    {
        if (curr_frame.a_pressed) projected_angle += Game::Data::tank_types[player_vector[ID].tank_type].rot_speed * curr_frame.delta_time;
        if (curr_frame.d_pressed) projected_angle -= Game::Data::tank_types[player_vector[ID].tank_type].rot_speed * curr_frame.delta_time;

        if (curr_frame.w_pressed)
        {
            double displacement = (Game::Data::tank_types[player_vector[ID].tank_type].move_speed * curr_frame.delta_time);
            dx += (displacement*cos(player_vector[ID].angle));
            dy += (displacement*sin(player_vector[ID].angle));
        }
        if (curr_frame.s_pressed)
        {
            double displacement = (Game::Data::tank_types[player_vector[ID].tank_type].move_speed * curr_frame.delta_time);
            dx -= (displacement*cos(player_vector[ID].angle));
            dy -= (displacement*sin(player_vector[ID].angle));
        }
    }
    else
    {

    }

    // Handle collision detection
    bool player_colliding_x=false;
    bool player_colliding_y=false;
    bool player_colliding=false;
    bool player_colliding_rot=false;

    Rectangle collider = Physics::make_rect({
        .x = player_vector[ID].position.x,
        .y = player_vector[ID].position.y, // BOTTOM LEFT (after make_rect() otherwise center)
        .width = static_cast<float>(Game::Data::tank_types[player_vector[ID].tank_type].width),
        .height = static_cast<float>(Game::Data::tank_types[player_vector[ID].tank_type].height),
    });
    double scanning_radius = 1.5*(sqrt((Game::Data::tank_types[player_vector[ID].tank_type].width)*(Game::Data::tank_types[player_vector[ID].tank_type].width) + (Game::Data::tank_types[player_vector[ID].tank_type].height)*(Game::Data::tank_types[player_vector[ID].tank_type].height)));
    size_t scanning_radius_tiles = (size_t)ceil(scanning_radius/Maps::maps[map_num].tile_width_units);
    // Player wall
    size_t pos_y = Maps::maps[map_num].map_height_tiles - (size_t)(player_vector[ID].position.y/Maps::maps[map_num].tile_width_units) - 1;
    size_t pos_x = (size_t)(player_vector[ID].position.x/Maps::maps[map_num].tile_width_units);
    // size_t pos_idx = ((Maps::maps[map_num].map_width_tiles)*pos_y) + pos_x;
    for(size_t wall_y = pos_y-std::min(pos_y, scanning_radius_tiles); wall_y<pos_y+scanning_radius_tiles; wall_y++)
    {
        for(size_t wall_x = pos_x-std::min(pos_x, scanning_radius_tiles); wall_x<pos_x+scanning_radius_tiles; wall_x++)
        {
            size_t wall_idx = ((Maps::maps[map_num].map_width_tiles)*wall_y) + wall_x;
            if((wall_x>=0) && (wall_y>=0) && (wall_x<=Maps::maps[map_num].map_width_tiles-1) && (wall_y<=Maps::maps[map_num].map_height_tiles-1) && Maps::maps[map_num].walls[wall_idx]==0)
            {
                Rectangle wall = {
                    .x = static_cast<float>(wall_x)*(Maps::maps[map_num].tile_width_units),
                    .y = static_cast<float>(Maps::maps[map_num].map_height_tiles - wall_y - 1)*(Maps::maps[map_num].tile_width_units), // TOP LEFT
                    .width = (Maps::maps[map_num].tile_width_units),
                    .height = (Maps::maps[map_num].tile_width_units),
                };
                if(!player_colliding) player_colliding = Physics::sat_collision_detection(wall, 0, {collider.x+static_cast<float>(dx), collider.y+static_cast<float>(dy), collider.width, collider.height}, player_vector[ID].angle);
                if(!player_colliding_x) player_colliding_x = Physics::sat_collision_detection(wall, 0, {collider.x+static_cast<float>(dx), collider.y, collider.width, collider.height}, player_vector[ID].angle);
                if(!player_colliding_y) player_colliding_y = Physics::sat_collision_detection(wall, 0, {collider.x, collider.y+static_cast<float>(dy), collider.width, collider.height}, player_vector[ID].angle);
                
                if(player_colliding && player_colliding_x && player_colliding_y){break;}
            }
            if(player_colliding && player_colliding_x && player_colliding_y){break;}
        }
        if(player_colliding && player_colliding_x && player_colliding_y){break;}
    }
    if(!(player_colliding && player_colliding_x && player_colliding_y))
    {
        // Player player
        for(size_t i=0; i<12; i++)
        {
            if (!player_vector[i].exists) continue;
            if (!player_vector[i].is_alive) continue;
            if(i!=ID)
            {
                Rectangle other_player_collider = Physics::make_rect({
                    .x = player_vector[i].position.x,
                    .y = player_vector[i].position.y,
                    .width = static_cast<float>(Game::Data::tank_types[player_vector[i].tank_type].width),
                    .height = static_cast<float>(Game::Data::tank_types[player_vector[i].tank_type].height),
                });
                if(!player_colliding) player_colliding = Physics::sat_collision_detection({collider.x+static_cast<float>(dx), collider.y+static_cast<float>(dy), collider.width, collider.height}, player_vector[ID].angle, other_player_collider, player_vector[i].angle);
                if(!player_colliding_x) player_colliding_x = Physics::sat_collision_detection({collider.x+static_cast<float>(dx), collider.y, collider.width, collider.height}, player_vector[ID].angle, other_player_collider, player_vector[i].angle);
                if(!player_colliding_y) player_colliding_y = Physics::sat_collision_detection({collider.x, collider.y+static_cast<float>(dy), collider.width, collider.height}, player_vector[ID].angle, other_player_collider, player_vector[i].angle);

                if(player_colliding && player_colliding_x && player_colliding_y){break;}
            }
            if(player_colliding && player_colliding_x && player_colliding_y){break;}
        }
    }

    // Resolve collision (translation)
    if(!player_colliding)
    {
        player_vector[ID].position.x += dx;
        player_vector[ID].position.y += dy;
    }
    else if(player_colliding && player_colliding_x && !player_colliding_y)
    {
        player_vector[ID].position.y += dy;
    }
    else if(player_colliding && !player_colliding_x && player_colliding_y)
    {
        player_vector[ID].position.x += dx;
    }
    else
    {
        // Do nothing
    }


    // Rotation
    collider = Physics::make_rect({
        .x = player_vector[ID].position.x,
        .y = player_vector[ID].position.y, // BOTTOM LEFT (after make_rect() otherwise center)
        .width = static_cast<float>(Game::Data::tank_types[player_vector[ID].tank_type].width),
        .height = static_cast<float>(Game::Data::tank_types[player_vector[ID].tank_type].height),
    });
    // Player wall
    pos_y = Maps::maps[map_num].map_height_tiles - (size_t)(player_vector[ID].position.y/Maps::maps[map_num].tile_width_units) - 1;
    pos_x = (size_t)(player_vector[ID].position.x/Maps::maps[map_num].tile_width_units);
    
    
    // size_t pos_idx = ((Maps::maps[map_num].map_width_tiles)*pos_y) + pos_x;
    for(size_t wall_y = pos_y-std::min(pos_y, scanning_radius_tiles); wall_y<pos_y+scanning_radius_tiles; wall_y++)
    {
        for(size_t wall_x = pos_x-std::min(pos_x, scanning_radius_tiles); wall_x<pos_x+scanning_radius_tiles; wall_x++)
        {
            
            size_t wall_idx = ((Maps::maps[map_num].map_width_tiles)*wall_y) + wall_x;
            if((wall_x>=0) && (wall_y>=0) && (wall_x<=Maps::maps[map_num].map_width_tiles-1) && (wall_y<=Maps::maps[map_num].map_height_tiles-1) && Maps::maps[map_num].walls[wall_idx]==0)
            {
                Rectangle wall = {
                    .x = static_cast<float>(wall_x)*(Maps::maps[map_num].tile_width_units),
                    .y = static_cast<float>(Maps::maps[map_num].map_height_tiles - wall_y - 1)*(Maps::maps[map_num].tile_width_units),
                    .width = (Maps::maps[map_num].tile_width_units),
                    .height = (Maps::maps[map_num].tile_width_units),
                };
                if(!player_colliding_rot) player_colliding_rot = Physics::sat_collision_detection(wall, 0, collider, projected_angle);

                if(player_colliding_rot){break;}
            }
            if(player_colliding_rot){break;}
        }
        if(player_colliding_rot){break;}
    }
    if(!player_colliding_rot)
    {
        // Player player
        for(size_t i=0; i<12; i++)
        {
            if (!player_vector[i].exists) continue;
            if (!player_vector[i].is_alive) continue;
            if(i!=ID)
            {
                Rectangle other_player_collider = Physics::make_rect({
                    .x = player_vector[i].position.x,
                    .y = player_vector[i].position.y,
                    .width = static_cast<float>(Game::Data::tank_types[player_vector[i].tank_type].width),
                    .height = static_cast<float>(Game::Data::tank_types[player_vector[i].tank_type].height),
                });
                if(!player_colliding_rot) player_colliding_rot = Physics::sat_collision_detection(collider, projected_angle, other_player_collider, player_vector[i].angle);

                if(player_colliding_rot){break;}
            }
            if(player_colliding_rot){break;}
        }
    }

    if(!player_colliding_rot)
    {
        player_vector[ID].angle = projected_angle;
    }
}

void Game::GameState::set_gun_angle()
{
    size_t ID = curr_frame.player_id;
    double expected_gun_angle = normalize_angle(atan2(curr_frame.mouse_position_screen.y - (Maps::maps[map_num].tiles_in_screen_y*Maps::maps[map_num].tile_width_units)/2, curr_frame.mouse_position_screen.x - (Maps::maps[map_num].tiles_in_screen_x*Maps::maps[map_num].tile_width_units)/2));
    double angle_difference = normalize_angle(expected_gun_angle - player_vector[ID].gun_angle);
    
    // Take shortest path to targeted gun_angle
    {
        double max_angular_displacement = (Game::Data::gun_types[player_vector[ID].gun_type].rot_speed * curr_frame.delta_time);
        if (angle_difference < PI)
        {
            player_vector[ID].gun_angle += max_angular_displacement;
            // Stop jittering by preventing overshoot
            if (player_vector[ID].gun_angle > expected_gun_angle && player_vector[ID].gun_angle - expected_gun_angle < PI){
                player_vector[ID].gun_angle = expected_gun_angle;
            }
            
        }
        else
        {
            player_vector[ID].gun_angle -= max_angular_displacement;
            // Stop jittering by preventing overshoot
            if (player_vector[ID].gun_angle < expected_gun_angle && expected_gun_angle - player_vector[ID].gun_angle < PI){
                player_vector[ID].gun_angle = expected_gun_angle;
            }
        }
    }
    player_vector[ID].gun_angle = normalize_angle(player_vector[ID].gun_angle);
}

void Game::GameState::handle_shots()
{
    size_t ID = curr_frame.player_id;
    if(curr_frame.lmb_pressed)
    {
        if(player_vector[ID].time_since_last_shot<Game::Data::gun_types[player_vector[ID].gun_type].reload_time)
        {
           player_vector[ID].time_since_last_shot += curr_frame.delta_time; 
        }
        else
        {
            player_vector[ID].time_since_last_shot = 0;
            switch (player_vector[ID].gun_type){
                case 0: // Gun behaviour
                {
                    bool bullet_colliding = false;
                    Vector2 contact_point = {
                        player_vector[ID].position.x + static_cast<float>(Game::Data::gun_types[player_vector[ID].gun_type].range*cos(player_vector[ID].gun_angle)), 
                        player_vector[ID].position.y + static_cast<float>(Game::Data::gun_types[player_vector[ID].gun_type].range*sin(player_vector[ID].gun_angle))};
                    Vector2 curr_contact_point = contact_point;
                    float min_dis = Vector2Distance(contact_point, player_vector[ID].position);
                    float cur_dis;
                    for(size_t wall_y = 0; wall_y<Maps::maps[map_num].map_height_tiles; wall_y++)
                    {
                        for(size_t wall_x = 0; wall_x<Maps::maps[map_num].map_width_tiles; wall_x++)
                        {
                            size_t wall_idx = ((Maps::maps[map_num].map_width_tiles)*wall_y) + wall_x;
                            if (wall_idx >= Maps::maps[map_num].walls.size()){
                                continue;
                            }
                            if((wall_x>=0) && (wall_y>=0) && (wall_x<=Maps::maps[map_num].map_width_tiles-1) && (wall_y<=Maps::maps[map_num].map_height_tiles-1) && Maps::maps[map_num].walls[wall_idx]==0)
                            {
                                Rectangle wall = {
                                    .x = static_cast<float>(wall_x)*(Maps::maps[map_num].tile_width_units),
                                    .y = static_cast<float>(Maps::maps[map_num].map_height_tiles - wall_y - 1)*(Maps::maps[map_num].tile_width_units),
                                    .width = (Maps::maps[map_num].tile_width_units),
                                    .height = (Maps::maps[map_num].tile_width_units),
                                };
                                bullet_colliding = Physics::CheckCollisionRay2dRect(player_vector[ID].position, player_vector[ID].gun_angle, wall, &curr_contact_point);
                                cur_dis = Vector2Distance(curr_contact_point, player_vector[ID].position);
                                if(bullet_colliding){
                                    
                                    if(cur_dis<min_dis)
                                    {
                                        contact_point = curr_contact_point;
                                        min_dis = cur_dis;
                                    }
                                }
                            }
                        }
                    }

                    curr_contact_point = contact_point;
                    min_dis = Vector2Distance(contact_point, player_vector[ID].position);
                    

                    bullet_colliding=false;
                    int hitting_idx = -1;
                    for(size_t i=0; i<12; i++)
                    {   
                        if(i!=ID && player_vector[i].is_alive)
                        {
                            Rectangle other_player_collider = Physics::make_rect({
                                .x = player_vector[i].position.x,
                                .y = player_vector[i].position.y,
                                .width = static_cast<float>(Game::Data::tank_types[player_vector[i].tank_type].width),
                                .height = static_cast<float>(Game::Data::tank_types[player_vector[i].tank_type].height),
                            });

                            bullet_colliding = Physics::CheckCollisionRay2dRectEx(player_vector[ID].position, player_vector[ID].gun_angle, other_player_collider, player_vector[i].angle, &curr_contact_point);
                            cur_dis = Vector2Distance(curr_contact_point, player_vector[ID].position);
                            
                            if(bullet_colliding){
                                if(cur_dis < min_dis)
                                {
                                    min_dis = cur_dis;
                                    contact_point = curr_contact_point;
                                    hitting_idx = i;
                                }
                            }
                        }
                    }
                    if(hitting_idx >= 0){
                        if(player_vector[hitting_idx].health <= Game::Data::gun_types[player_vector[ID].gun_type].dmg){
                            player_vector[hitting_idx].is_alive = false;
                            player_vector[hitting_idx].health = 0;
                            player_vector[ID].score +=100;
                        }
                        else{
                            player_vector[hitting_idx].health -= Game::Data::gun_types[player_vector[ID].gun_type].dmg;
                        }
                    }

                    player_vector[ID].ray_contact = contact_point;

                    break;
                }

                case 1:
                {
                    Game::GameState::Projectile new_projectile;
                    new_projectile.type = 0;
                    new_projectile.angle = player_vector[ID].gun_angle;
                    new_projectile.position =  {
                            .x = player_vector[ID].position.x + static_cast<float>((Game::Data::gun_types[player_vector[ID].gun_type].width + Game::Data::projectile_types[0].width + 1)*cos(player_vector[ID].gun_angle)),
                            .y = player_vector[ID].position.y + static_cast<float>((Game::Data::gun_types[player_vector[ID].gun_type].width + Game::Data::projectile_types[0].width + 1)*sin(player_vector[ID].gun_angle)),
                        };
                    new_projectile.time_alive = 0;
                    new_projectile.shot_id = ID;
                    projectile_vector.push_back(new_projectile);
                    break;
                }

                case 2:
                {
                    Game::GameState::Explosion new_explosion;
                    new_explosion.position =  {
                            .x = curr_frame.mouse_position_screen.x - (Maps::maps[map_num].tiles_in_screen_x*Maps::maps[map_num].tile_width_units)/2 + player_vector[ID].position.x,
                            .y = curr_frame.mouse_position_screen.y - (Maps::maps[map_num].tiles_in_screen_y*Maps::maps[map_num].tile_width_units)/2 + player_vector[ID].position.y,
                        };
                    new_explosion.time_alive = 0;
                    new_explosion.shot_id = ID;
                    new_explosion.type = 0;
                    explosion_vector.push_back(new_explosion);
                    break;
                }
            }
        }
    }
    else
    {
        player_vector[ID].time_since_last_shot += curr_frame.delta_time;
    }
}

void Game::GameState::update_projectiles(float delta_time)
{

    for(auto projectile_itr = projectile_vector.begin(); projectile_itr != projectile_vector.end();)
    {
        bool not_colliding = true;

        Rectangle collider = Physics::make_rect({(*projectile_itr).position.x, (*projectile_itr).position.y, static_cast<float>(Game::Data::projectile_types[(*projectile_itr).type].width), static_cast<float>(Game::Data::projectile_types[(*projectile_itr).type].height)});
        size_t pos_y = Maps::maps[map_num].map_height_tiles - (size_t)(collider.y/Maps::maps[map_num].tile_width_units) - 1;
        size_t pos_x = (size_t)(collider.x/Maps::maps[map_num].tile_width_units);
        // size_t pos_idx = ((Maps::maps[map_num].map_width_tiles)*pos_y) + pos_x;
        
        // double scanning_radius = 1.5*(sqrt((Game::Data::projectile_types[(*projectile_itr).type].width)*(Game::Data::projectile_types[(*projectile_itr).type].width) + (Game::Data::projectile_types[(*projectile_itr).type].height)*(Game::Data::projectile_types[(*projectile_itr).type].height)));
        size_t scanning_radius_tiles = 2;

        for(size_t wall_y = pos_y-std::min(pos_y, scanning_radius_tiles); wall_y<pos_y+scanning_radius_tiles; wall_y++)
        {
            for(size_t wall_x = pos_x-std::min(pos_x, scanning_radius_tiles); wall_x<pos_x+scanning_radius_tiles; wall_x++)
            {
                size_t wall_idx = ((Maps::maps[map_num].map_width_tiles)*wall_y) + wall_x;
                if((wall_x>=0) && (wall_y>=0) && (wall_x<=Maps::maps[map_num].map_width_tiles-1) && (wall_y<=Maps::maps[map_num].map_height_tiles-1) && Maps::maps[map_num].walls[wall_idx]==0)
                {
                    Rectangle wall = {
                        .x = static_cast<float>(wall_x)*(Maps::maps[map_num].tile_width_units),
                        .y = static_cast<float>(Maps::maps[map_num].map_height_tiles - wall_y - 1)*(Maps::maps[map_num].tile_width_units),
                        .width = (Maps::maps[map_num].tile_width_units),
                        .height = (Maps::maps[map_num].tile_width_units),
                    };
                    not_colliding = !(Physics::sat_collision_detection(wall, 0, collider, (*projectile_itr).angle));
                    if(!not_colliding){break;}
                }
            }
            if(!not_colliding){
                break;
            }
        }

        if(not_colliding)
        {
            for(size_t j=0; j<8; j++)
            {
                if (!player_vector[j].is_alive) continue;
                Rectangle other_player_collider = Physics::make_rect({
                    .x = player_vector[j].position.x,
                    .y = player_vector[j].position.y,
                    .width = static_cast<float>(Game::Data::tank_types[player_vector[j].tank_type].width),
                    .height = static_cast<float>(Game::Data::tank_types[player_vector[j].tank_type].height),
                });
                not_colliding = !(Physics::sat_collision_detection(collider, (*projectile_itr).angle, other_player_collider, player_vector[j].angle));
                if(!not_colliding){
                    if(player_vector[j].health <= Game::Data::projectile_types[(*projectile_itr).type].dmg){
                        player_vector[j].is_alive = false;
                        player_vector[j].health = 0;
                        player_vector[(*projectile_itr).shot_id].score +=100;
                    }
                    else{
                        player_vector[j].health -= Game::Data::projectile_types[(*projectile_itr).type].dmg;
                    }
                    break;
                }
            }
        }

        if((*projectile_itr).time_alive<Game::Data::projectile_types[(*projectile_itr).type].lifetime && not_colliding)
        {
            double displacement = (Game::Data::projectile_types[(*projectile_itr).type].mov_speed * delta_time);
            (*projectile_itr).position.x += (displacement*cos((*projectile_itr).angle));
            (*projectile_itr).position.y += (displacement*sin((*projectile_itr).angle));
            (*projectile_itr).time_alive += delta_time;

            ++projectile_itr;
        }
        else
        {
            projectile_itr = projectile_vector.erase(projectile_itr);
        }
    }
    return;
}

void Game::GameState::update_explosions(float delta_time)
{
    for(auto explosion_itr = explosion_vector.begin(); explosion_itr != explosion_vector.end();)
    {
        if((*explosion_itr).time_alive<Game::Data::explosion_types[(*explosion_itr).type].lifetime)
        {
            (*explosion_itr).time_alive += delta_time;
            ++explosion_itr;
            continue;
        }
        for(size_t j=0; j<12; j++)
        {   
            if(!player_vector[j].is_alive) continue;
            
            Rectangle player_collider = {
                .x = player_vector[j].position.x,
                .y = player_vector[j].position.y,
                .width = static_cast<float>(Game::Data::tank_types[player_vector[j].tank_type].width),
                .height = static_cast<float>(Game::Data::tank_types[player_vector[j].tank_type].height),
            };

            bool explosion_colliding = Physics::CheckCollisionCircleRectPro((*explosion_itr).position, Game::Data::explosion_types[(*explosion_itr).type].radius, player_collider, player_vector[j].angle);
            if(!explosion_colliding) continue;
            
            if(player_vector[j].health <= Game::Data::explosion_types[(*explosion_itr).type].dmg){
                player_vector[j].is_alive = false;
                player_vector[j].health = 0;
                if(j!=(*explosion_itr).shot_id) player_vector[(*explosion_itr).shot_id].score += 100;
            }
            else{
                player_vector[j].health -= Game::Data::explosion_types[(*explosion_itr).type].dmg;
            }

        }
        explosion_itr = explosion_vector.erase(explosion_itr);
    }
    return;
}

long double Game::GameState::curtime()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> ctime = std::chrono::high_resolution_clock().now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(ctime-GameState::start_time);
    return duration.count();
}