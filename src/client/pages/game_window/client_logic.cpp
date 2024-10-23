#include "client_logic.hpp"

LogicUtils::PlayerPacket LogicUtils::player_packet;
LogicUtils::LivePlayerData LogicUtils::player_data;
LogicUtils::LivePlayerData LogicUtils::projected_data;
LogicUtils::GunStats LogicUtils::gun_data;
LogicUtils::HullStats LogicUtils::hull_data;
LogicUtils::ViewportData LogicUtils::viewport_data;
LogicUtils::CrosshairData LogicUtils::crosshair_data;

std::vector<LogicUtils::PlayerPacket> LogicUtils::old_state;

Texture2D LogicUtils::player_spritesheet;
Image LogicUtils::player_spritesheet_image;
Utils::AnimationController *LogicUtils::player_controller;
size_t LogicUtils::player_idle_idx;
size_t LogicUtils::player_moving_idx;
Utils::AnimationController *LogicUtils::gun_controller;
size_t LogicUtils::gun_idle_idx;
size_t LogicUtils::gun_shot_idx;
Texture2D LogicUtils::map;
Image LogicUtils::map_image;

void LogicUtils::init_state(int max_players)
{
    for(int i=0; i<max_players; i++)
    {
        PlayerPacket packet;
        packet = {
            .ID=i,
            .position_absolute = {0,0},
            .player_angle = 0,
            .gun_angle = 0,
            .has_shot = false,
            .gun_dmg = 1,
            .health = 5,
            .is_alive = false,
            .is_connected = false,
        };
        old_state.push_back(packet);
    }
};

void LogicUtils::handle_movement(float delta_time)
{
    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
    {
        player_controller->play(player_moving_idx, false);
        if (IsKeyDown(KEY_A)) projected_data.angle -= hull_data.player_rot_speed * delta_time;
        if (IsKeyDown(KEY_D)) projected_data.angle += hull_data.player_rot_speed * delta_time;

        if (IsKeyDown(KEY_W))
        {
            double displacement = (hull_data.player_speed * delta_time);
            projected_data.position.x += (displacement*cos(projected_data.angle));
            projected_data.position.y += (displacement*sin(projected_data.angle));
            viewport_data.offset.x += (displacement*cos(projected_data.angle));
            viewport_data.offset.y += (displacement*sin(projected_data.angle));
        }
        if (IsKeyDown(KEY_S))
        {
            double displacement = (hull_data.player_speed * delta_time);
            projected_data.position.x -= (displacement*cos(projected_data.angle));
            projected_data.position.y -= (displacement*sin(projected_data.angle));
            viewport_data.offset.x -= (displacement*cos(projected_data.angle));
            viewport_data.offset.y -= (displacement*sin(projected_data.angle));
        }
    }
    else
    {
        player_controller->play(player_idle_idx);
    }
}

double LogicUtils::normalize_angle(double angle)
{
    while (angle >= 2 * PI) angle -= 2.0 * PI;
    while (angle < 0) angle += 2.0 * PI;
    return angle;
}

void LogicUtils::set_gun_angle(float delta_time)
{
    gun_data.expected_gun_angle = normalize_angle(atan2(crosshair_data.mouse_position.y - (Maps::map1.tiles_in_screen_x*Maps::map1.tile_width_units)/2, crosshair_data.mouse_position.x - (Maps::map1.tiles_in_screen_y*Maps::map1.tile_width_units)/2));
    double angle_difference = normalize_angle(gun_data.expected_gun_angle - gun_data.gun_angle);
    
    // Take shortest path to targeted gun_angle
    {
        double max_angular_displacement = (gun_data.gun_rot_speed * delta_time);
        if (angle_difference < PI)
        {
            gun_data.gun_angle += max_angular_displacement;
            // Stop jittering by preventing overshoot
            if (gun_data.gun_angle > gun_data.expected_gun_angle && gun_data.gun_angle - gun_data.expected_gun_angle < PI){
                gun_data.gun_angle = gun_data.expected_gun_angle;
            }
            
        }
        else
        {
            gun_data.gun_angle -= max_angular_displacement;
            // Stop jittering by preventing overshoot
            if (gun_data.gun_angle < gun_data.expected_gun_angle && gun_data.expected_gun_angle - gun_data.gun_angle < PI){
                gun_data.gun_angle = gun_data.expected_gun_angle;
            }
        }
    }
    gun_data.gun_angle = normalize_angle(gun_data.gun_angle);
}


void LogicUtils::set_tracker(float delta_time)
{
    // Targeted radial distance (mouse distance)
    crosshair_data.mouse_distance = sqrt(((crosshair_data.mouse_position.x-(Maps::map1.tiles_in_screen_x*Maps::map1.tile_width_units)/2)*(crosshair_data.mouse_position.x-(Maps::map1.tiles_in_screen_x*Maps::map1.tile_width_units)/2) + (crosshair_data.mouse_position.y-(Maps::map1.tiles_in_screen_y*Maps::map1.tile_width_units)/2)*(crosshair_data.mouse_position.y-(Maps::map1.tiles_in_screen_y*Maps::map1.tile_width_units)/2)));
    
    // Move tracker radially
    if (crosshair_data.mouse_distance - crosshair_data.tracker_distance > 0){
        crosshair_data.tracker_distance += crosshair_data.tracker_radial_speed * delta_time;
        if (crosshair_data.tracker_distance > crosshair_data.mouse_distance){
            crosshair_data.tracker_distance = crosshair_data.mouse_distance;
        }
    } else{
        crosshair_data.tracker_distance -= crosshair_data.tracker_radial_speed * delta_time;
        if (crosshair_data.tracker_distance < crosshair_data.mouse_distance){
            crosshair_data.tracker_distance = crosshair_data.mouse_distance;
        }
    }

    // Prevent tracker from being on tank
    crosshair_data.tracker_distance = std::max((float)crosshair_data.tracker_distance, gun_data.gun_rectangle.width-5);

    // Update coordinates
    crosshair_data.tracker_position.x = crosshair_data.tracker_distance*cos(gun_data.gun_angle);
    crosshair_data.tracker_position.y = crosshair_data.tracker_distance*sin(gun_data.gun_angle);
}

void LogicUtils::handle_tank_collision()
{
    // Handle collision detection
    bool player_colliding=false;

    Rectangle collider = {
        .x = projected_data.position.x,
        .y = projected_data.position.y,
        .width = hull_data.player_rectangle.width,
        .height = hull_data.player_rectangle.height
    };

    // Player wall
    size_t pos_y = (size_t)(projected_data.position.y/Maps::map1.tile_width_units);
    size_t pos_x = (size_t)(projected_data.position.x/Maps::map1.tile_width_units);
    size_t pos_idx = ((Maps::map1.map_width_tiles)*pos_y) + pos_x;
    
    for(size_t wall_y = pos_y-8*(Maps::map1.map_width_tiles); wall_y<pos_y+8*(Maps::map1.map_width_tiles); wall_y++)
    {
        for(size_t wall_x = pos_x-8; wall_x<pos_x+8; wall_x++)
        {
            size_t wall_idx = ((Maps::map1.map_width_tiles)*wall_y) + wall_x;
            if((Vector2Distance({(float)wall_x, (float)wall_y}, {(float)pos_x, (float)pos_y})<=8) && Maps::map1.walls[wall_idx]==0)
            {
                Rectangle wall = {
                    .x = (float)(wall_x)*(Maps::map1.tile_width_units),
                    .y = (float)(wall_y)*(Maps::map1.tile_width_units),
                    .width = (Maps::map1.tile_width_units),
                    .height = (Maps::map1.tile_width_units),
                };
                player_colliding = Physics::sat_collision_detection(wall, 0, collider, projected_data.angle);
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
            if(i!=player_packet.ID)
            {
                Rectangle other_player_collider = {
                    .x = old_state[i].position_absolute.x,
                    .y = old_state[i].position_absolute.y,
                    .width = hull_data.player_rectangle.width,
                    .height = hull_data.player_rectangle.height,
                };
                player_colliding = Physics::sat_collision_detection(collider, projected_data.angle, other_player_collider, old_state[i].player_angle);
                if(player_colliding){break;}
            }
            if(player_colliding){break;}
        }
    }
    if(!player_colliding)
    {
        player_data.position = projected_data.position;
        player_data.angle = projected_data.angle;
    }
};