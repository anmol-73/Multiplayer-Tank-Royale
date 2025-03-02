#include "game_renderer.hpp"
#include <iostream>

Pages::GameRenderer::GameRenderer()
{
    map_images.assign(Maps::map_count, {});
    for (size_t i = 0; i < Maps::map_count; ++i){
        map_images[i].path = Maps::maps[i].resource_path;
    }
}

void Pages::GameRenderer::draw(const Game::GameState& gs, int player_id)
{
    camera.follow(gs.player_vector[player_id].position);

    { // draw map
        DrawTexturePro(
            map_images[gs.map_num].tex,
            camera.viewport(),
            {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())},
            {0, 0},
            0,
            WHITE
        );
    }

    { // draw tanks
        for (size_t i = 0; i < gs.player_vector.size(); ++ i){
            if (!(gs.player_vector[i].exists and gs.player_vector[i].is_alive)) continue;

            DrawRectanglePro(
                camera.transform(
                    {
                        gs.player_vector[i].position.x, gs.player_vector[i].position.y,
                        static_cast<float>(Game::Data::tank_types[i].width), static_cast<float>(Game::Data::tank_types[i].height)
                    }
                ), camera.scale(Vector2{
                    static_cast<float>(Game::Data::tank_types[i].width/2), static_cast<float>(Game::Data::tank_types[i].height/2)
                }), -RAD2DEG*gs.player_vector[i].angle, WHITE
            );
        }
    }
}

void Pages::GameRenderer::prepare(size_t _map_idx, size_t _player_tank_type)
{
    map_index = _map_idx;
    tank_index = _player_tank_type;

    camera.init({Maps::maps[map_index].width(), Maps::maps[map_index].height()}, {Maps::maps[map_index].vwidth(), Maps::maps[map_index].vheight()}, {static_cast<float>(Game::Data::tank_types[tank_index].width), static_cast<float>(Game::Data::tank_types[tank_index].height)});
}

void Pages::GameRenderer::load_async()
{
    for (size_t i = 0; i < Maps::map_count; ++i){
        map_images[i].load_im();
    }
}

void Pages::GameRenderer::load_sync()
{
    for (size_t i = 0; i < Maps::map_count; ++i){
        map_images[i].load_tex();
    }
}

void Pages::GameRenderer::cleanup_async()
{
    for (size_t i = 0; i < Maps::map_count; ++i){
        map_images[i].unload_im();
    }
}

void Pages::GameRenderer::cleanup_sync()
{
    for (size_t i = 0; i < Maps::map_count; ++i){
        map_images[i].unload_tex();
    }
}
