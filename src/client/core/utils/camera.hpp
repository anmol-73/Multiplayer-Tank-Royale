#ifndef H_UTILS_CAMERA
#define H_UTILS_CAMERA

#include <raylib.h>
#include <raymath.h>
#include <cassert>

namespace Utils
{
    struct Camera{
    public:
        /** Initialize the camera based on the map*/
        void init(Vector2 map_size, Vector2 viewport_size, Vector2 player_size);

        /** Update the camera position based on the player position */
        void follow(Vector2 player_position);

        /** Converts a point in world space to camera space */
        Vector2 transform(Vector2 point);
        
        /** Converts a rect in world space to camera space */
        Rectangle transform(Rectangle rect);

        /** Scales a size from world units to camera units */
        float scale(float size);
        Vector2 scale(Vector2 size);
        Rectangle scale(Rectangle size);

        /** Undoes scale */
        Vector2 descale(Vector2 size);
        Rectangle descale(Rectangle size);

        Rectangle viewport();

        private:
        /** The position of the camera in world space */
        Vector2 position;
        /** The size of the viewport of the camera in world size */
        Vector2 viewport_size;
        /** The total size of the map */
        Vector2 map_size;
        /** The of the window we are working with */
        Vector2 half_screen_size;

        float scaling_factor;
        Vector2 player_size;

    };
    
} // namespace Utils


#endif