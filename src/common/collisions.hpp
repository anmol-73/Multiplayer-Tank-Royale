#ifndef HEADER_COLLISIONS
#define HEADER_COLLISIONS

#include <vector>
#include <raylib.h>
#include <raymath.h>

namespace Physics
{
    // Function to rotate a point around a center by an angle (in radians)
    Vector2 rotate_point(Vector2 point, Vector2 center, float angle_rad);

    // Function to get the corners of a rectangle
    std::vector<Vector2> get_corners(Rectangle rectangle, float angle_rad);

    // Function to project a polygon onto an axis
    std::pair<float, float> project_polygon(std::vector<Vector2> corners, Vector2 axis);

    // Function to check if two projections overlap
    bool projections_overlap(std::pair<float, float> proj1, std::pair<float, float> proj2);

    // SAT Collision Detection for two rectangles
    bool sat_collision_detection(Rectangle rec1, float angle_rad_1, Rectangle rec2, float angle_rad_2);

    void SwapFloatValue(float *a, float *b);

    bool CheckCollisionRay2dRectEx(Vector2 origin, float dirn_angle, Rectangle rect, float rec_angle,Vector2* intersection);
    bool CheckCollisionRay2dRect(Vector2 origin, float dirn_angle, Rectangle rect, Vector2* intersection);

    bool CheckCollisionCircleRectPro(Vector2 center, float radius, Rectangle rec, float rec_angle);
} // namespace Physics


#endif