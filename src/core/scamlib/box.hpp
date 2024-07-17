#pragma once

#include <raylib.h>

namespace Scamlib
{
    namespace Draw
    {

        struct BoxDrawParameters{
            Vector2 position; // The position to draw in 
            Vector2 translate; // A delta relative to size of the box
            Vector2 size; // The size of the box
            
            Color fill = {255, 255, 255, 0};

            struct BorderParameters{
                float width = 0;
                Color color = BLACK;
                struct BorderRadius{
                    float top_left;
                    float top_right;
                    float bottom_left;
                    float bottom_right;
                    BorderRadius(float radius): top_left(radius), top_right(radius), bottom_left(radius), bottom_right(radius) {};
                    BorderRadius(float tl, float tr, float bl, float br): top_left(tl), top_right(tr), bottom_left(bl), bottom_right(br) {};
                    BorderRadius(): BorderRadius(0) {};
                } radius = {};
            } border;
        };

        /**
         * Renders a rectangle
         * 
         * RETURNS: The size of the bounding box of the box
         */
        Rectangle box(BoxDrawParameters params);

        /**
         * Renders a rectangle without a border radius
         * 
         * RETURNS: The size of the bounding box of the box
         */
        Rectangle __box_no_radius(BoxDrawParameters params);

        /**
         * Renders a rectangle with border radius
         * NOTE: This is less efficient than no border radius
         * 
         * RETURNS: The size of the bounding box of the box
         */
        Rectangle __box_with_radius(BoxDrawParameters params);
    } // namespace Draw
    
} // namespace Scamlib

#include "box.cpp"
