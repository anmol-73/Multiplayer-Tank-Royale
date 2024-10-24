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

void Physics::SwapFloatValue(float *a, float *b){
    float t = *a;
    *a = *b;
    *b = t;
}

/*
    DESCRIPTION : A function to detect Collisions between a Ray and a Rectangle.
    - Also provides additonal data such as the contact point and contact normal and the probale collison points
    - PARAMETER ray_origin : A Vector2 defining the origin/start point of the Ray
    - PARAMETER ray_dir : A Vector2 defining the direction of the Ray
    - PARAMETER targetRect : The target rectangle against which we are checking the collision
    - PARAMETER contact_point(Pointer) : A Vector2 that stores the contact point if the collision occurs with the rectangle
    - PARAMETER contact_normal(Pointer) : A Vector2 that stores the contact normal of the rectangle surface if the collision occurs with the rectangle
    - PARAMETER near_contact_time(Pointer) : A float that stores the parametric contact time of the rectangle surface if the collision occurs with the rectangle
    - PARAMETER probableContactPoints : An array of Vector2's that stores the probale near and far contact points with the ray and rectanlge if the there is a chance of collision
    RETURNS : A bool indicating wether the collision has occured or not.
    - Also updates the contact_point, contact_normal and the probableContactPoints pointers
*/
bool Physics::RayVsRect2D(const Vector2 ray_origin, const Vector2 ray_dir, const Rectangle targetRect, Vector2* contact_point){
    Vector2* contact_normal;
    float* near_contact_time;
    Vector2 probableContactPoints[2];
    /*
    * The t in the P(t) = P + D.t
    * Where t is the parametric variable to plot the near collison point using the parametric line equation (P(t) = P + D.t)
    * Where P is the Position Vector of the Ray and D is the Direciton Vector of the Ray
    */
    float t_hit_near;

    /*
    * Calculate intersection points with rectangle bounding axes
    * Parametric 't' for Near (X,Y) and Far (X,Y)
    */
    float delta_t1_X = targetRect.x - ray_origin.x;
    float t_hit_near_X = (delta_t1_X / ray_dir.x);

    float delta_t1_Y = targetRect.y - ray_origin.y;
    float t_hit_near_Y = (delta_t1_Y / ray_dir.y);

    float delta_t2_X = targetRect.x + targetRect.width - ray_origin.x;
    float t_hit_far_X = (delta_t2_X / ray_dir.x);

    float delta_t2_Y = targetRect.y + targetRect.height - ray_origin.y;
    float t_hit_far_Y = (delta_t2_Y / ray_dir.y);

    /*
    * Sort the distances to maintain Affine uniformity
    * i.e. sort the near and far axes of the rectangle in the appropritate order from the POV of ray_origin
    */
    if (t_hit_near_X > t_hit_far_X) SwapFloatValue(&t_hit_near_X, &t_hit_far_X);
    if (t_hit_near_Y > t_hit_far_Y) SwapFloatValue(&t_hit_near_Y, &t_hit_far_Y);

    // As there is no chance of collision i.e the paramteric line cannot pass throguh the rectangle the probable points are empty
    probableContactPoints[0] = (Vector2){0, 0};
    probableContactPoints[1] = (Vector2){0, 0};

    /*
    * Check the order of the near and far points
    * if they satisfy the below condition the line will pass through the rectanle (It didn't yet)
    * if not return out of the function as it will not pass through
    */
    if(!(t_hit_near_X < t_hit_far_Y && t_hit_near_Y < t_hit_far_X)) return false;

    /*
    * If the parametric line passes through the rectangle calculate the parametric 't'
    * the 't' should be such that it must lie on both the Line/Ray and the Rectangle
    * Use the condition below to calculate the 'tnear' and 'tfar' that gives the near and far collison parametric t
    */
    *near_contact_time = std::max(t_hit_near_X, t_hit_near_Y);
    float t_hit_far = std::min(t_hit_far_X, t_hit_far_Y);

    // Use the parametric t values calculated above and subsitute them in the parametric equation to get the near and far contact points
    float Hit_Near_X_Position = ray_origin.x + (ray_dir.x * (*near_contact_time));
    float Hit_Near_Y_Position = ray_origin.y + (ray_dir.y * (*near_contact_time));

    float Hit_Far_X_Position = ray_origin.x + (ray_dir.x * t_hit_far);
    float Hit_Far_Y_Position = ray_origin.y + (ray_dir.y * t_hit_far);

    // Debugging the calculations
    #ifdef RAY_COLLISION_CALCULATION_DEBUG_STATS
        printf("The delta_t1_X is : %f | t_hit_near_X is : %f | ray_dir.x is : %f | rayOrigin.x is : %f | Hit_Near_X_Position is : %f \n", delta_t1_X, t_hit_near_X, ray_dir.x, ray_origin.x, Hit_Near_X_Position);
        printf("The delta_t1_Y is : %f | t_hit_near_Y is : %f | ray_dir.y is : %f | rayOrigin.y is : %f | Hit_Near_Y_Position is : %f \n", delta_t1_Y, t_hit_near_Y, ray_dir.y, ray_origin.y, Hit_Near_Y_Position);
        printf("t_hit_near is : %f \n", *near_contact_time);
        printf("The delta_t2_X is : %f | t_hit_far_X is : %f | ray_dir.x is : %f | rayOrigin.x is : %f | Hit_Far_X_Position is : %f \n", delta_t2_X, t_hit_far_X, ray_dir.x, ray_origin.x, Hit_Far_X_Position);
        printf("The delta_t2_Y is : %f | t_hit_far_Y is : %f | ray_dir.y is : %f | rayOrigin.y is : %f | Hit_Far_Y_Position is : %f \n", delta_t2_Y, t_hit_far_Y, ray_dir.y, ray_origin.y, Hit_Far_Y_Position);
        printf("t_hit_far is : %f \n", t_hit_far);
    #endif

    // Generate Vectors using the near and far collision points
    Vector2 Near_Hit_Vector = (Vector2){Hit_Near_X_Position, Hit_Near_Y_Position};
    Vector2 Far_Hit_Vector = (Vector2){Hit_Far_X_Position, Hit_Far_Y_Position};
    // Since we are sure that the line will pass through the rectanle upadte the probable near and far points
    probableContactPoints[0] = Near_Hit_Vector;
    probableContactPoints[1] = Far_Hit_Vector;

    /*
    * Check wether the parametric 't' values are withing certain bounds to guarantee that the probable collision has actually occured
    * i.e. If the below conditions are true only then the Ray has passed through the Rectangle
    * if not it still can pass but it did not yet
    */
    if(t_hit_far < 0 || t_hit_near > 1) return false;

    // Now Update the actual contact point
    *contact_point = (Vector2){Hit_Near_X_Position, Hit_Near_Y_Position};

    // Update the contact normal of the Ray with the Rectangle surface
    if(t_hit_near_X > t_hit_near_Y){
        if(ray_dir.x < 0) *contact_normal = (Vector2){1, 0};
        else *contact_normal = (Vector2){-1, 0};
    }
    else if(t_hit_near_X < t_hit_near_Y){
        if(ray_dir.y < 0) *contact_normal = (Vector2){0, 1};
        else  *contact_normal = (Vector2){0, -1};
    }
    // Since the contact has definitely occured return true
    return true;
}