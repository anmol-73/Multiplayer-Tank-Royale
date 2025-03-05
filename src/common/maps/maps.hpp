#ifndef H_COM_MAPS
#define H_COM_MAPS

#include <vector>
#include <string>
#include <stdlib.h>
#include <raylib.h>


namespace Maps{

    struct MapData{
        size_t tiles_in_screen_x;
        size_t tiles_in_screen_y;
        size_t map_width_tiles;
        size_t map_height_tiles;
        float tile_width_units;
        int unique_tiles; // Walls always 0 for now
        std::vector<int> walls;
        std::vector<Vector2> spawnpoints;

        float width();
        float height();
        
        float vwidth();
        float vheight();

        std::string resource_path;
        std::string name;
    };

    constexpr size_t map_count = 1;
    extern MapData maps[map_count];

    //void setmap(MapData map, int map_no);
}


#endif