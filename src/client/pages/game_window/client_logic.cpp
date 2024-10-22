#include "client_logic.hpp"

void LogicUtils::handle_movement(float delta_time)
{
    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
    {
        if (IsKeyDown(KEY_A)) player_data.angle += hull_data.player_rot_speed * delta_time;
        if (IsKeyDown(KEY_D)) player_data.angle -= hull_data.player_rot_speed * delta_time;

        if (IsKeyDown(KEY_W))
        {
            double displacement = (hull_data.player_speed * delta_time);
            player_data.position.x += (displacement*cos(player_data.angle));
            player_data.position.y -= (displacement*sin(player_data.angle));
            viewport_data.offset.x += (displacement*cos(player_data.angle));
            viewport_data.offset.y -= (displacement*sin(player_data.angle));
        }
        if (IsKeyDown(KEY_S))
        {
            double displacement = (hull_data.player_speed * delta_time);
            player_data.position.x -= (displacement*cos(player_data.angle));
            player_data.position.y += (displacement*sin(player_data.angle));
            viewport_data.offset.x -= (displacement*cos(player_data.angle));
            viewport_data.offset.y += (displacement*sin(player_data.angle));
        }
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
    gun_data.expected_gun_angle = normalize_angle(atan2(crosshair_data.mouse_position.y - player_data.position.y - viewport_data.offset.y, crosshair_data.mouse_position.x - player_data.position.x - viewport_data.offset.x));
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
    crosshair_data.mouse_distance = sqrt(((crosshair_data.mouse_position.x-player_data.position.x-viewport_data.offset.x)*(crosshair_data.mouse_position.x-player_data.position.x-viewport_data.offset.x)) + ((crosshair_data.mouse_position.y-player_data.position.y-viewport_data.offset.y)*(crosshair_data.mouse_position.y-player_data.position.y-viewport_data.offset.y)));
    
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
    crosshair_data.tracker_distance = std::max((float)crosshair_data.tracker_distance, gun_data.gun_rectangle.height-5);

    // Update coordinates
    crosshair_data.tracker_position.x = crosshair_data.tracker_distance*cos(gun_data.gun_angle);
    crosshair_data.tracker_position.y = crosshair_data.tracker_distance*sin(gun_data.gun_angle);
}