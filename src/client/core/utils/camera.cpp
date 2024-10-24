#include "camera.hpp"

void Utils::Camera::init(Vector2 map_size_, Vector2 viewport_size_)
{
    map_size = map_size_;
    viewport_size = viewport_size_;
    half_screen_size = Vector2Scale({(float)GetScreenWidth(), (float)GetScreenHeight()}, 0.5f);
    
    scaling_factor = Vector2Divide({(float)GetScreenWidth(), (float)GetScreenHeight()}, viewport_size);
    
}

void Utils::Camera::follow(Vector2 player_position)
{
    // We don't do that lag business, but if you wanted to you could do it here :)
    position = player_position;
}

Vector2 Utils::Camera::transform(Vector2 point)
{
    return Vector2Add(
        scale(Vector2Subtract(point, position)), // Position wrt to camera in screen units
        half_screen_size // Camera is at the middle of the screen always
    );   
}

Rectangle Utils::Camera::transform(Rectangle rect)
{
    return Rectangle{
        .x = (rect.x - position.x) * scaling_factor.x + half_screen_size.x,
        .y = (rect.y - position.y) * scaling_factor.y + half_screen_size.y,
        .width = rect.width * scaling_factor.x,
        .height = rect.height * scaling_factor.y,
    };
}

Vector2 Utils::Camera::scale(Vector2 size)
{
    return Vector2Multiply(size, scaling_factor);
}
Rectangle Utils::Camera::scale(Rectangle size)
{
    return {
        size.x * scaling_factor.x,
        size.y * scaling_factor.y,
        size.width * scaling_factor.x,
        size.height * scaling_factor.y
    };
}
