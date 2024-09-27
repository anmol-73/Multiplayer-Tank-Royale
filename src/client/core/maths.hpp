#ifndef HEADER_MATHS
#define HEADER_MATHS

#include <math.h>
#include <time.h>
#include <raymath.h>

namespace Maths
{
    /**
     * Set seed to some random thing
     */
    void init_rand();

    /**
     * Generates a random number between lo and hi
     * NOTE: Expects lo and hi to be positive and hi > lo
     */
    double random(double lo, double hi);

    /**
     * Returns true if the segment intersects with the circle
     */
    bool segment_intersects_with_circle(Vector2 seg_start, Vector2 seg_end, Vector2 circ_center, float circ_radius);
} // namespace Maths

#endif