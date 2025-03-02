#include "game_renderer.hpp"

Pages::GameRenderer::GameRenderer()
{
    map_images.assign(Maps::map_count, {});
    for (size_t i = 0; i < Maps::map_count; ++i){
        map_images[i].path = Maps::maps[i].resource_path;
    }
}

void Pages::GameRenderer::draw(const Game::GameState& gs)
{
}

void Pages::GameRenderer::prepare(size_t _map_idx, size_t _player_tank_type)
{
    map_index = _map_idx;
    tank_index = _player_tank_type;
    
    // camera.init({Maps::maps[map_index].width(), Maps::maps[map_index].height()}, {Maps::maps[map_index].vwidth(), Maps::maps[map_index].vheight()}, )
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
