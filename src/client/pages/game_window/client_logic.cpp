#include "client_logic.hpp"
#include <iostream>

std::vector<float> LogicUtils::old_timestamps;
LogicUtils::PlayerPacket LogicUtils::player_packet;
LogicUtils::LivePlayerData LogicUtils::player_data;
LogicUtils::LivePlayerData LogicUtils::projected_data;
LogicUtils::GunStats LogicUtils::gun_data;
LogicUtils::HullStats LogicUtils::hull_data;
LogicUtils::ViewportData LogicUtils::viewport_data;
LogicUtils::CrosshairData LogicUtils::crosshair_data;
std::vector<bool> LogicUtils::did_shoots;
std::vector<Vector2> LogicUtils::contact_pointsss;
float LogicUtils::timesince_lastshot;
int LogicUtils::map_idx{};

Utils::Animation* LogicUtils::player_idle;
Utils::Animation* LogicUtils::player_moving;
Utils::Animation* LogicUtils::gun_idle;
Utils::Animation* LogicUtils::gun_shot;

std::vector<LogicUtils::PlayerPacket> LogicUtils::old_state;

Texture2D LogicUtils::player_spritesheet;
Image LogicUtils::player_spritesheet_image;
std::vector<Utils::AnimationController> LogicUtils::player_controllers;
std::vector<Utils::AnimationController> LogicUtils::gun_controllers;
size_t LogicUtils::player_idle_idx;
size_t LogicUtils::player_moving_idx;
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
            .is_idle = true,
            .ID=i,
            .position_absolute = {0,0},
            .player_angle = 0,
            .gun_angle = 0,
            .health = 5,
            .has_shot = false,
            .is_alive = false,
            .is_connected = false,
        };
        old_state.push_back(packet);
    }
};

void LogicUtils::update_state(PlayerPacket *received_state)
{
    for (size_t i =0; i < old_state.size();++i){
        old_timestamps[i] = old_state[i].last_shot;
    }
    // TODO: Use proper constant here
    old_state = std::vector(received_state, received_state + Networking::Message::Room::MAX_ROOM_SIZE);
    // memcpy(&player_packet ,&old_state[player_packet.ID], sizeof(player_packet));
    for (size_t i =0; i < old_state.size();++i){
        did_shoots[i] = old_state[i].last_shot != old_timestamps[i];
        if (did_shoots[i])
        contact_pointsss[i] = old_state[i].closest_wall_hit;
    }
}
void LogicUtils::set_packet() {
    player_packet.gun_angle = gun_data.gun_angle;
    // player_packet.has_shot = gun_data.has_shot; Tis is like set automagically
    player_packet.is_idle = (Vector2Equals(player_data.position, player_packet.position_absolute));
    player_packet.health = player_data.health;
    player_packet.ID = Global::ServiceProviders::room_client.get_id();
    player_packet.is_alive = player_data.is_alive;
    player_packet.is_connected = true;
    player_packet.player_angle = player_data.angle;
    player_packet.position_absolute = player_data.position;
    player_packet.player_dmg = 100;
};

void LogicUtils::handle_movement(float delta_time)
{
    size_t self = Global::ServiceProviders::room_client.get_id();
    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
    {
        player_controllers[self].play(player_moving_idx, false);
        if (IsKeyDown(KEY_A)) projected_data.angle -= hull_data.player_rot_speed * delta_time;
        if (IsKeyDown(KEY_D)) projected_data.angle += hull_data.player_rot_speed * delta_time;

        if (IsKeyDown(KEY_W))
        {
            double displacement = (hull_data.player_speed * delta_time);
            projected_data.position.x += (displacement*cos(projected_data.angle));
            projected_data.position.y += (displacement*sin(projected_data.angle));
            viewport_data.projected_offset.x += (displacement*cos(projected_data.angle));
            viewport_data.projected_offset.y += (displacement*sin(projected_data.angle));
        }
        if (IsKeyDown(KEY_S))
        {
            double displacement = (hull_data.player_speed * delta_time);
            projected_data.position.x -= (displacement*cos(projected_data.angle));
            projected_data.position.y -= (displacement*sin(projected_data.angle));
            viewport_data.projected_offset.x -= (displacement*cos(projected_data.angle));
            viewport_data.projected_offset.y -= (displacement*sin(projected_data.angle));
        }
    }
    else
    {
        player_controllers[self].play(player_idle_idx);
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
    gun_data.expected_gun_angle = normalize_angle(atan2(crosshair_data.mouse_position.y - (Maps::maps[0].tiles_in_screen_y*Maps::maps[0].tile_width_units)/2, crosshair_data.mouse_position.x - (Maps::maps[0].tiles_in_screen_x*Maps::maps[0].tile_width_units)/2));
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
    crosshair_data.mouse_distance = sqrt(((crosshair_data.mouse_position.x-(Maps::maps[0].tiles_in_screen_x*Maps::maps[0].tile_width_units)/2)*(crosshair_data.mouse_position.x-(Maps::maps[0].tiles_in_screen_x*Maps::maps[0].tile_width_units)/2) + (crosshair_data.mouse_position.y-(Maps::maps[0].tiles_in_screen_y*Maps::maps[0].tile_width_units)/2)*(crosshair_data.mouse_position.y-(Maps::maps[0].tiles_in_screen_y*Maps::maps[0].tile_width_units)/2)));
    
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

bool LogicUtils::handle_tank_collision()
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
    size_t pos_y = (size_t)((projected_data.position.y+hull_data.player_rectangle.height/2)/Maps::maps[0].tile_width_units);
    size_t pos_x = (size_t)((projected_data.position.x+hull_data.player_rectangle.width/2)/Maps::maps[0].tile_width_units);
    size_t pos_idx = ((Maps::maps[0].map_width_tiles)*pos_y) + pos_x;
    size_t rad = (size_t)(sqrt((hull_data.player_rectangle.width*hull_data.player_rectangle.width)/(Maps::maps[0].tile_width_units*Maps::maps[0].tile_width_units) + (hull_data.player_rectangle.height*hull_data.player_rectangle.height)/(Maps::maps[0].tile_width_units*Maps::maps[0].tile_width_units)));
    
    for(size_t wall_y = pos_y-std::min(rad, pos_y); wall_y<pos_y+rad; wall_y++)
    {
        for(size_t wall_x = pos_x-std::min(rad, pos_x); wall_x<pos_x+rad; wall_x++)
        {
            size_t wall_idx = ((Maps::maps[0].map_width_tiles)*wall_y) + wall_x;
            if (wall_idx >= Maps::maps[0].walls.size()){
                continue;
            }
            if((Vector2Distance({(float)wall_x, (float)wall_y}, {(float)pos_x, (float)pos_y})<=rad) && Maps::maps[0].walls[wall_idx]==0)
            // if(Maps::maps[0].walls[wall_idx]==0)
            {
                Rectangle wall = {
                    .x = (float)(wall_x)*(Maps::maps[0].tile_width_units),
                    .y = (float)(wall_y)*(Maps::maps[0].tile_width_units),
                    .width = (Maps::maps[0].tile_width_units),
                    .height = (Maps::maps[0].tile_width_units),
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
        for(int i=0; i<12; i++)
        {
            if (!old_state[i].is_alive)continue;
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
        viewport_data.offset = viewport_data.projected_offset;
        player_data.angle = projected_data.angle;
    }
    else{
        projected_data.position = player_data.position;
        viewport_data.projected_offset = viewport_data.offset;
        projected_data.angle = player_data.angle;
    }
    
    return player_colliding;
};

void LogicUtils::LivePlayerData::init(Vector2 position_)
{
    position = position_;
    angle = 0;
    health = 5;
}

void LogicUtils::HullStats::init()
{
    player_speed = 120.0f;
    player_rot_speed = PI/2;
    player_colliding = false;
}

void LogicUtils::GunStats::init()
{
    gun_angle = 0;
    gun_rot_speed = 1;
    gun_dmg = 1;
    has_shot = false;
    bullet_range = 100000;
}

void LogicUtils::CrosshairData::init()
{
    tracker_position = Vector2();
    tracker_radial_speed = 500;
    tracker_radius = 10;
    tracker_distance = 0;
}
