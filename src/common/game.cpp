#include "game.hpp"

void Game::GameState::apply_frame(const Game::Frame& frame)
{
    curr_frame = frame;
    handle_movement();
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
    double dx;
    double dy;
    if(curr_frame.a_pressed || curr_frame.d_pressed || curr_frame.s_pressed || curr_frame.w_pressed)
    {
        if (curr_frame.a_pressed) projected_angle += Game::Data::tank_types[player_vector[ID].tank_type].rot_speed * curr_frame.delta_time;
        if (curr_frame.d_pressed) projected_angle -= Game::Data::tank_types[player_vector[ID].tank_type].rot_speed * curr_frame.delta_time;

        if (curr_frame.w_pressed)
        {
            double displacement = (Game::Data::tank_types[player_vector[ID].tank_type].move_speed * curr_frame.delta_time);
            dx += (displacement*cos(projected_angle));
            dy += (displacement*sin(projected_angle));
        }
        if (curr_frame.s_pressed)
        {
            double displacement = (Game::Data::tank_types[player_vector[ID].tank_type].move_speed * curr_frame.delta_time);
            dx -= (displacement*cos(projected_angle));
            dy -= (displacement*sin(projected_angle));
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

    Rectangle collider = {
        .x = player_vector[ID].position.x,
        .y = player_vector[ID].position.y,
        .width = Game::Data::tank_types[player_vector[ID].tank_type].width,
        .height = Game::Data::tank_types[player_vector[ID].tank_type].height,
    };
    double scanning_radius = 1.5*(sqrt((Game::Data::tank_types[player_vector[ID].tank_type].width)*(Game::Data::tank_types[player_vector[ID].tank_type].width) + (Game::Data::tank_types[player_vector[ID].tank_type].height)*(Game::Data::tank_types[player_vector[ID].tank_type].height)));
    size_t scanning_radius_tiles = (size_t)ceil(scanning_radius/Maps::maps[map_num].tile_width_units);
    // Player wall
    size_t pos_y = (size_t)(player_vector[ID].position.y/Maps::maps[map_num].tile_width_units);
    size_t pos_x = (size_t)(player_vector[ID].position.x/Maps::maps[map_num].tile_width_units);
    size_t pos_idx = ((Maps::maps[map_num].map_width_tiles)*pos_y) + pos_x;
    for(size_t wall_y = pos_y-scanning_radius_tiles*(Maps::maps[map_num].map_width_tiles); wall_y<pos_y+scanning_radius_tiles*(Maps::maps[map_num].map_width_tiles); wall_y++)
    {
        for(size_t wall_x = pos_x-scanning_radius_tiles; wall_x<pos_x+scanning_radius_tiles; wall_x++)
        {
            size_t wall_idx = ((Maps::maps[0].map_width_tiles)*wall_y) + wall_x;
            if((Vector2Distance({(float)wall_x, (float)wall_y}, {(float)pos_x, (float)pos_y})<=scanning_radius_tiles) && Maps::maps[0].walls[wall_idx]==0)
            {
                Rectangle wall = {
                    .x = (float)(wall_x)*(Maps::maps[0].tile_width_units),
                    .y = (float)(wall_y)*(Maps::maps[0].tile_width_units),
                    .width = (Maps::maps[0].tile_width_units),
                    .height = (Maps::maps[0].tile_width_units),
                };
                if(!player_colliding) player_colliding = Physics::sat_collision_detection(wall, 0, {collider.x+(float)dx, collider.y+(float)dy, collider.width, collider.height}, player_vector[ID].angle);
                if(!player_colliding_x) player_colliding_x = Physics::sat_collision_detection(wall, 0, {collider.x+(float)dx, collider.y, collider.width, collider.height}, player_vector[ID].angle);
                if(!player_colliding_y) player_colliding_y = Physics::sat_collision_detection(wall, 0, {collider.x, collider.y+(float)dy, collider.width, collider.height}, player_vector[ID].angle);

                if(player_colliding && player_colliding_x && player_colliding_y){break;}
            }
            if(player_colliding && player_colliding_x && player_colliding_y){break;}
        }
        if(player_colliding && player_colliding_x && player_colliding_y){break;}
    }
    if(!(player_colliding && player_colliding_x && player_colliding_y))
    {
        // Player player
        for(int i=0; i<num_of_players; i++)
        {
            if (!player_vector[i].is_alive) continue;
            if(i!=ID)
            {
                Rectangle other_player_collider = {
                    .x = player_vector[i].position.x,
                    .y = player_vector[i].position.y,
                    .width = Game::Data::tank_types[player_vector[i].tank_type].width,
                    .height = Game::Data::tank_types[player_vector[i].tank_type].height,
                };
                if(!player_colliding) player_colliding = Physics::sat_collision_detection({collider.x+(float)dx, collider.y+(float)dy, collider.width, collider.height}, player_vector[ID].angle, other_player_collider, player_vector[i].angle);
                if(!player_colliding_x) player_colliding_x = Physics::sat_collision_detection({collider.x+(float)dx, collider.y, collider.width, collider.height}, player_vector[ID].angle, other_player_collider, player_vector[i].angle);
                if(!player_colliding_y) player_colliding_y = Physics::sat_collision_detection({collider.x, collider.y+(float)dy, collider.width, collider.height}, player_vector[ID].angle, other_player_collider, player_vector[i].angle);

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
    else if(!player_colliding && !player_colliding_x && player_colliding_y)
    {
        player_vector[ID].position.x += dx;
    }


    // Rotation
    collider.x = player_vector[ID].position.x;
    collider.y = player_vector[ID].position.y;
    scanning_radius = 1.5*(sqrt((Game::Data::tank_types[player_vector[ID].tank_type].width)*(Game::Data::tank_types[player_vector[ID].tank_type].width) + (Game::Data::tank_types[player_vector[ID].tank_type].height)*(Game::Data::tank_types[player_vector[ID].tank_type].height)));
    scanning_radius_tiles = (size_t)ceil(scanning_radius/Maps::maps[map_num].tile_width_units);
    // Player wall
    pos_y = (size_t)(player_vector[ID].position.y/Maps::maps[map_num].tile_width_units);
    pos_x = (size_t)(player_vector[ID].position.x/Maps::maps[map_num].tile_width_units);
    pos_idx = ((Maps::maps[map_num].map_width_tiles)*pos_y) + pos_x;
    for(size_t wall_y = pos_y-scanning_radius_tiles*(Maps::maps[map_num].map_width_tiles); wall_y<pos_y+scanning_radius_tiles*(Maps::maps[map_num].map_width_tiles); wall_y++)
    {
        for(size_t wall_x = pos_x-scanning_radius_tiles; wall_x<pos_x+scanning_radius_tiles; wall_x++)
        {
            size_t wall_idx = ((Maps::maps[0].map_width_tiles)*wall_y) + wall_x;
            if((Vector2Distance({(float)wall_x, (float)wall_y}, {(float)pos_x, (float)pos_y})<=scanning_radius_tiles) && Maps::maps[0].walls[wall_idx]==0)
            {
                Rectangle wall = {
                    .x = (float)(wall_x)*(Maps::maps[0].tile_width_units),
                    .y = (float)(wall_y)*(Maps::maps[0].tile_width_units),
                    .width = (Maps::maps[0].tile_width_units),
                    .height = (Maps::maps[0].tile_width_units),
                };
                if(!player_colliding_rot) player_colliding = Physics::sat_collision_detection(wall, 0, collider, projected_angle);

                if(player_colliding_rot){break;}
            }
            if(player_colliding_rot){break;}
        }
        if(player_colliding_rot){break;}
    }
    if(!player_colliding_rot)
    {
        // Player player
        for(int i=0; i<num_of_players; i++)
        {
            if (!player_vector[i].is_alive) continue;
            if(i!=ID)
            {
                Rectangle other_player_collider = {
                    .x = player_vector[i].position.x,
                    .y = player_vector[i].position.y,
                    .width = Game::Data::tank_types[player_vector[i].tank_type].width,
                    .height = Game::Data::tank_types[player_vector[i].tank_type].height,
                };
                if(!player_colliding_rot) player_colliding = Physics::sat_collision_detection(collider, player_vector[ID].angle, other_player_collider, projected_angle);

                if(player_colliding_rot){break;}
            }
            if(player_colliding_rot){break;}
        }
    }

    // Resolve collision (rotation)
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
