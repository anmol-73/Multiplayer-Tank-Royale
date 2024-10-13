#include "maths.hpp"

void Maths::init_rand()
{
    srand(static_cast <unsigned> (time(0)) + rand());
}

double Maths::random(double lo, double hi)
{
    return lo + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(hi-lo)));
}

bool Maths::segment_intersects_with_circle(Vector2 seg_start, Vector2 seg_end, Vector2 circ_center, float circ_radius)
{
    // https://github.com/Pakz001/Raylib-Examples/blob/master/Collision_-_LinesegCircleCollide.c
    float xd = 0.0f;
    float yd = 0.0f;
    float t = 0.0f;
    float d = 0.0f;
    float dx = 0.0f;
    float dy = 0.0f;

    float x1 = seg_start.x;
    float x2 = seg_end.x;
    float y1 = seg_start.y;
    float y2 = seg_end.y;
    float xc = circ_center.x;
    float yc = circ_center.y;
    float r = circ_radius * circ_radius;

    dx = x2 - x1;
    dy = y2 - y1;

    if (dx != 0 && dy != 0){
        t = ((yc - y1) * dy + (xc - x1) * dx) / (dy * dy + dx * dx);
    } else{
        t = 1e9;
    }


    if(0 <= t && t <= 1)
    {
        xd = x1 + t * dx;
        yd = y1 + t * dy;

        d = ((xd - xc) * (xd - xc) + (yd - yc) * (yd - yc));
        return d <= r;
    }
    
    d = ((xc - x1) * (xc - x1) + (yc - y1) * (yc - y1));
    if(d <= r)
    {
        return true;
    }

    d = ((xc - x2) * (xc - x2) + (yc - y2) * (yc - y2));
    return (d <= r);
}

int Maths::gcd(int a, int b)
{
   if(b == 0) {
        return a;
    }
    else {
        return gcd(b, a % b);
    }
}