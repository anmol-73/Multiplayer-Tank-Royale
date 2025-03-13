#include "camera.hpp"
#include <iostream>

void Utils::Camera::init(Vector2 map_size_, Vector2 viewport_size_)
{
    map_size = map_size_;
    viewport_size = viewport_size_;
    half_screen_size = Vector2Scale({(float)GetScreenWidth(), -(float)GetScreenHeight()}, 0.5f);
    
    auto scaling_factor_v = Vector2Divide({(float)GetScreenWidth(), (float)GetScreenHeight()}, viewport_size);
    std::cout << scaling_factor_v.x << ' ' << scaling_factor_v.y << "!" << std::endl;

    scaling_factor = scaling_factor_v.x;
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
        .width = rect.width * scaling_factor,
        .height = rect.height * scaling_factor,
    };
}

float Utils::Camera::scale(float size)
{
    return size * scaling_factor;
}

Vector2 Utils::Camera::scale(Vector2 size)
{
    return Vector2Scale(size, scaling_factor);
}
Rectangle Utils::Camera::scale(Rectangle size)
{
    return {
        size.x * scaling_factor,
        size.y * scaling_factor,
        size.width * scaling_factor,
        size.height * scaling_factor
    };
}

Vector2 Utils::Camera::descale(Vector2 size)
{
    return Vector2Scale(size, 1 / scaling_factor);
}

Rectangle Utils::Camera::descale(Rectangle size)
{
    return {
        size.x / scaling_factor,
        size.y / scaling_factor,
        size.width / scaling_factor,
        size.height / scaling_factor
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