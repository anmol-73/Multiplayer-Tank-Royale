#include "collisions.hpp"

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