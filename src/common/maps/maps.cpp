#include "maps.hpp"

// void Maps::setmap(MapData map, int map_no){
//             switch (map_no)
//             {
//             case 1:
//                 map.map_length = maps[0].map_length;
//                 map.map_width = maps[0].map_width;
//                 map.tile_width = GetScreenWidth()/16;
//                 map.tile = maps[0].tile;
//                 break;
            
//             default:
//                 break;
//             }
//         }

float Maps::MapData::width()
{
    return map_width_tiles * tile_width_units;
}

float Maps::MapData::height()
{
    return map_height_tiles * tile_width_units;
}

float Maps::MapData::vwidth()
{
    return tiles_in_screen_x * tile_width_units;
}

float Maps::MapData::vheight()
{
    return tiles_in_screen_y * tile_width_units;
}
