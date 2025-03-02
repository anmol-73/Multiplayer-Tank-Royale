#include "collisions.hpp"
#include <iostream>

Rectangle Physics::make_rect(Rectangle bad_rect)
{
    return {bad_rect.x - bad_rect.width/2, bad_rect.y-bad_rect.height/2, bad_rect.width, bad_rect.height};
}

Vector2 Physics::rotate_point(Vector2 point, Vector2 center, float angle_rad)
{
    Vector2 vec = Vector2Subtract(point, center);
    vec = Vector2Rotate(vec, angle_rad);
    return Vector2Add(vec, center);
}

std::vector<Vector2> Physics::get_corners(Rectangle rectangle, float angle_rad)
{
    float half_width = rectangle.width / 2;
    float half_height = rectangle.height / 2;
    // Center coords
    float x = rectangle.x + half_width;
    float y = rectangle.y + half_height;
    Vector2 center = {.x=x,.y=y};

    // Corners before rotation (relative to center)
    std::vector<Vector2> corners = {
        {x - half_width, y - half_height},  // top-left
        {x + half_width, y - half_height},  // top-right
        {x + half_width, y + half_height},  // bottom-right
        {x - half_width, y + half_height}   // bottom-left
    };

    // Rotate corners around the center
    for (auto& corner : corners) {
        corner = Physics::rotate_point(corner, center, angle_rad);
    }
    return corners;
}

// Function to project a polygon onto an axis
std::pair<float, float> Physics::project_polygon(std::vector<Vector2> corners, Vector2 axis)
{
    float min_proj = INFINITY;
    float max_proj = -INFINITY;
    for (const auto& corner : corners) {
        float proj = corner.x * axis.x + corner.y * axis.y;  // dot product
        min_proj = std::min(min_proj, proj);
        max_proj = std::max(max_proj, proj);
    }
    return {min_proj, max_proj};
}

// Function to check if two projections overlap
bool Physics::projections_overlap(std::pair<float, float> proj1, std::pair<float, float> proj2) {
    return !(proj1.second < proj2.first || proj2.second < proj1.first);
}

// SAT Collision Detection for two rectangles
bool Physics::sat_collision_detection(Rectangle rec1, float angle_rad_1, Rectangle rec2, float angle_rad_2)
{
    std::vector<Vector2> corners1 = get_corners(rec1, angle_rad_1);
    std::vector<Vector2> corners2 = get_corners(rec2, angle_rad_2);
    std::vector<Vector2> axes;

    // Get axes (normals to edges) for rect1
    for (int i = 0; i < 4; ++i) {
        Vector2 edge = {corners1[i].x - corners1[(i + 1) % 4].x, corners1[i].y - corners1[(i + 1) % 4].y};
        Vector2 axis = Vector2Normalize({-edge.y, edge.x});  // Perpendicular axis
        axes.push_back(axis);
    }

    // Get axes (normals to edges) for rect2
    for (int i = 0; i < 4; ++i) {
        Vector2 edge = {corners2[i].x - corners2[(i + 1) % 4].x, corners2[i].y - corners2[(i + 1) % 4].y};
        Vector2 axis = Vector2Normalize({-edge.y, edge.x});  // Perpendicular axis
        axes.push_back(axis);
    }

    // Check for overlap on all axes
    for (const auto& axis : axes) {
        std::pair<float, float> proj1 = Physics::project_polygon(corners1, axis);
        std::pair<float, float> proj2 = Physics::project_polygon(corners2, axis);
        if (!Physics::projections_overlap(proj1, proj2)) {
            return false;  // Separating axis found, no collision
        }
    }

    return true;  // No separating axis found, collision detected
}

void Physics::SwapFloatValue(float *a, float *b){
    float t = *a;
    *a = *b;
    *b = t;
}

bool Physics::CheckCollisionRay2dRectEx(Vector2 origin, float dirn_angle, Rectangle rect, float rec_angle, Vector2 * intersection)
{
    Vector2 center = {rect.x + rect.width/2, rect.y + rect.height/2};
    Vector2 new_origin = rotate_point(origin, center, -rec_angle);
    Vector2 ixn;
    bool is_ixn = CheckCollisionRay2dRect(new_origin, dirn_angle - rec_angle, rect, &ixn);

    if (!is_ixn) return false;
    *intersection = rotate_point(ixn, center, rec_angle);
    return true;
}

bool Physics::CheckCollisionRay2dRect(Vector2 origin, float dirn_angle, Rectangle rect, Vector2* intersection)
{
    float minParam = -INFINITY, maxParam = INFINITY;
    Vector2 direction = {cos(dirn_angle), sin(dirn_angle)};
    if (direction.x != 0.0)
    {
        float txMin = (rect.x - origin.x) / direction.x;
        float txMax = ((rect.x + rect.width) - origin.x) / direction.x;

        minParam = fmax(minParam, fmin(txMin, txMax));
        maxParam = fmin(maxParam, fmax(txMin, txMax));
    }

    if (direction.y != 0.0)
    {
        float tyMin = (rect.y - origin.y) / direction.y;
        float tyMax = ((rect.y + rect.height) - origin.y) / direction.y;

        minParam = fmax(minParam, fmin(tyMin, tyMax));
        maxParam = fmin(maxParam, fmax(tyMin, tyMax));
    }

    // if maxParam < 0, ray is intersecting AABB, but the whole AABB is behind us
    if (maxParam < 0)
    {
        return false;
    }

    // if minParam > maxParam, ray doesn't intersect AABB
    if (minParam >= maxParam)
    {
        return false;
    }

    if (intersection != NULL)
    {
        *intersection = Vector2Add(origin, Vector2Scale(direction, minParam));
    }

    return true;
}

bool Physics::CheckCollisionCircleRectPro(Vector2 center, float radius, Rectangle rec, float rec_angle)
{
    center.x = center.x - rec.x;
    center.y = center.y - rec.y;
    rec.x = 0;
    rec.y = 0;
    float rotation_angle = rec_angle;
    Vector2 rotated_center = Vector2Rotate(center, rotation_angle);
    return CheckCollisionCircleRec(rotated_center, radius, rec);
}