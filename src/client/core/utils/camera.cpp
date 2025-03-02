#include "camera.hpp"


void Utils::Camera::init(Vector2 map_size_, Vector2 viewport_size_, Vector2 player_size_)
{
    map_size = map_size_;
    viewport_size = viewport_size_;
    player_size = player_size_; // TODO: Remove this shit
    half_screen_size = Vector2Scale({(float)GetScreenWidth(), -(float)GetScreenHeight()}, 0.5f);
    
    scaling_factor = Vector2Divide({(float)GetScreenWidth(), (float)GetScreenHeight()}, viewport_size);
}

void Utils::Camera::follow(Vector2 player_position)
{
    // We don't do that lag business, but if you wanted to you could do it here :)
    position = player_position;
}

Vector2 Utils::Camera::transform(Vector2 point)
{
    point = Vector2Add(
        scale(Vector2Subtract(point, position)), // Position wrt to camera in screen units
        half_screen_size // Camera is at the middle of the screen always
    );

    point.y = -point.y;
    return point;
}

Rectangle Utils::Camera::transform(Rectangle rect)
{
    Vector2 point = transform(Vector2{rect.x, rect.y});
    return Rectangle{
        .x = point.x,
        .y = point.y,
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

Rectangle Utils::Camera::viewport()
{
    return {
        position.x - viewport_size.x/2,
        (map_size.y - position.y) - viewport_size.y/2,
        viewport_size.x, viewport_size.y
    };
}