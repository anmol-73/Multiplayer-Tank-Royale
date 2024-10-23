/*************************************************************************
 * Walls are of type 0
 ***************************************************************************/

#ifndef HEADER_MAPS
#define HEADER_MAPS

#include <vector>
#include <stdlib.h>
#include <raylib.h>


namespace Maps{

    struct MapData{
        size_t tiles_in_screen_x;
        size_t tiles_in_screen_y;
        size_t map_width_tiles;
        size_t map_height_tiles;
        double tile_width_units;
        int unique_tiles; // Walls always 0 for now
        std::vector<int> walls;
    };

    extern MapData map1;

    //void setmap(MapData map, int map_no);
}


#endif