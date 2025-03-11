#include "game_renderer.hpp"
#include <iostream>

Pages::GameRenderer::GameRenderer()
{
    map_images.assign(Maps::map_count, {});
    for (size_t i = 0; i < Maps::map_count; ++i){
        map_images[i].path = Maps::maps[i].resource_path;
    }
}

void Pages::GameRenderer::draw(const Game::GameState& gs, int player_id, const std::vector<Communication::Game::PlayerIdentification>& pd)
{
    auto delta_time = GetFrameTime();
    if (gs.player_vector[player_id].is_alive){
        SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
    }

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
                        static_cast<float>(Game::Data::tank_types[gs.player_vector[i].tank_type].width), static_cast<float>(Game::Data::tank_types[gs.player_vector[i].tank_type].height)
                    }
                ), camera.scale(Vector2{
                    static_cast<float>(Game::Data::tank_types[gs.player_vector[i].tank_type].width/2), static_cast<float>(Game::Data::tank_types[gs.player_vector[i].tank_type].height/2)
                }), -RAD2DEG*gs.player_vector[i].angle, WHITE
            );
        }
    }

    { // draw gun
        for (size_t i = 0; i < gs.player_vector.size(); ++ i)
        {
            if((!gs.player_vector[i].exists) || (!gs.player_vector[i].is_alive)) continue;
            switch (gs.player_vector[i].gun_type)
            {
                case 0: // Gun behaviour
                {   // Draw traces
                    if(gs.player_vector[i].time_since_last_shot <= 0.1)
                    {
                        DrawLineEx(
                            camera.transform(gs.player_vector[i].position),
                            camera.transform(gs.player_vector[i].ray_contact),
                            4, WHITE
                        );
                    }
                    // Draw Gun
                    DrawRectanglePro(
                        camera.transform(
                            {
                                gs.player_vector[i].position.x, gs.player_vector[i].position.y,
                                static_cast<float>(Game::Data::gun_types[gs.player_vector[i].tank_type].width), static_cast<float>(Game::Data::gun_types[gs.player_vector[i].tank_type].height)
                            }
                        ), camera.scale(Vector2{
                            0, static_cast<float>(Game::Data::gun_types[gs.player_vector[i].tank_type].height/2)
                        }), -RAD2DEG*gs.player_vector[i].gun_angle, RED
                    );
                    // Draw tracker
                    DrawCircleLinesV(
                        Vector2Add(camera.transform(
                            gs.player_vector[player_id].position
                        ), crosshair_data.tracker_position),
                        crosshair_data.tracker_radius,
                        crosshair_data.circle_color
                    );
                    break;
                }

                case 1:
                {   // Draw Gun
                    DrawRectanglePro(
                        camera.transform(
                            {
                                gs.player_vector[i].position.x, gs.player_vector[i].position.y,
                                static_cast<float>(Game::Data::gun_types[gs.player_vector[i].tank_type].width), static_cast<float>(Game::Data::gun_types[gs.player_vector[i].tank_type].height)
                            }
                        ), camera.scale(Vector2{
                            0, static_cast<float>(Game::Data::gun_types[gs.player_vector[i].tank_type].height/2)
                        }), -RAD2DEG*gs.player_vector[i].gun_angle, RED
                    );
                    // Draw tracker
                    DrawCircleLinesV(
                        Vector2Add(camera.transform(
                            gs.player_vector[player_id].position
                        ), crosshair_data.tracker_position),
                        crosshair_data.tracker_radius,
                        crosshair_data.circle_color
                    );
                    break;
                }

                case 2:
                {
                    // Draw Gun
                    DrawRectanglePro(
                        camera.transform(
                            {
                                gs.player_vector[i].position.x, gs.player_vector[i].position.y,
                                static_cast<float>(Game::Data::gun_types[gs.player_vector[i].tank_type].width), static_cast<float>(Game::Data::gun_types[gs.player_vector[i].tank_type].height)
                            }
                        ), camera.scale(Vector2{
                            0, static_cast<float>(Game::Data::gun_types[gs.player_vector[i].tank_type].height/2)
                        }), -PI/2, RED
                    );
                    // Draw tracker
                    DrawCircleLinesV(
                        GetMousePosition(),
                        crosshair_data.tracker_radius,
                        crosshair_data.circle_color
                    );
                    break;
                }
            }
            prev_times_since_last_shot[i] = gs.player_vector[i].time_since_last_shot;
        }
    }

    { // draw projectiles
        for (size_t i = 0; i < gs.projectile_vector.size(); i++){
            DrawRectanglePro(
                camera.transform({
                    gs.projectile_vector[i].position.x, gs.projectile_vector[i].position.y,
                    static_cast<float>(Game::Data::projectile_types[gs.projectile_vector[i].type].width), static_cast<float>(Game::Data::projectile_types[gs.projectile_vector[i].type].height)
                }), 
                camera.scale(Vector2{
                    static_cast<float>(Game::Data::projectile_types[gs.projectile_vector[i].type].width/2), static_cast<float>(Game::Data::projectile_types[gs.projectile_vector[i].type].height/2)
                }), 
                -gs.projectile_vector[i].angle*RAD2DEG, RED
            );
        }
    }

    { // draw explosions
        for (size_t i = 0; i < gs.explosion_vector.size(); i++){
            
            auto position = camera.transform(gs.explosion_vector[i].position);
            DrawCircleGradient(
                position.x, position.y,
                camera.scale(Game::Data::explosion_types[gs.explosion_vector[i].type].radius), 
                MAGENTA, RED
            );
        }
    }

    { // draw health bar and name
        for (size_t i = 0; i < gs.player_vector.size(); ++ i)
        {
            if((!gs.player_vector[i].exists) || (!gs.player_vector[i].is_alive)) continue;
            float factor = static_cast<float>(gs.player_vector[i].health)/static_cast<float>(Game::Data::tank_types[gs.player_vector[i].tank_type].max_health);

            Rectangle health_bar = {
                .x = gs.player_vector[i].position.x,
                .y = gs.player_vector[i].position.y,
                .width = 100,
                .height = 10
            };

            health_bar = camera.transform(health_bar);
            Vector2 offset = camera.scale(Vector2{-50, -static_cast<float>(Game::Data::tank_types[gs.player_vector[i].tank_type].height)});
            
            health_bar.x += offset.x;
            health_bar.y += offset.y;
            DrawRectangleRounded(health_bar, 1.0, 6, RED);

            float actual_width = health_bar.width;

            health_bar.width *= factor; 
            DrawRectangleRounded(health_bar, 5, 6, GREEN);
            
            auto rec = DragonLib::Utils::Drawing::draw_text({
                .content = pd[i].name,
                .sdf=false,
                .font_size = Global::rem * 0.5f,
                .font_color = BLACK,
                .position = {.value = {health_bar.x + actual_width/2, health_bar.y + health_bar.height/2}, .mode = {DragonLib::UI::DrawParameters::ABSOLUTE, DragonLib::UI::DrawParameters::ABSOLUTE}},
            });
        }
    }

    { // Draw tracker
        switch (gs.player_vector[player_id].gun_type){
            case 0:
            case 1:
            {
                { // Tracker calculation
                    // Targeted radial distance (mouse distance)
                    Vector2 ppos = camera.transform(gs.player_vector[player_id].position);
                    crosshair_data.mouse_distance = Vector2Distance(crosshair_data.mouse_position, ppos);

                    // Move tracker radially
                    if (crosshair_data.mouse_distance - crosshair_data.tracker_distance > 0){
                        crosshair_data.tracker_distance += crosshair_data.tracker_radial_speed * delta_time;
                        if (crosshair_data.tracker_distance > crosshair_data.mouse_distance){
                            crosshair_data.tracker_distance = crosshair_data.mouse_distance;
                        }
                    } else{
                        crosshair_data.tracker_distance -= crosshair_data.tracker_radial_speed * delta_time;
                        if (crosshair_data.tracker_distance < crosshair_data.mouse_distance){
                            crosshair_data.tracker_distance = crosshair_data.mouse_distance;
                        }
                    }

                    // Prevent tracker from being on tank
                    crosshair_data.tracker_distance = std::max(crosshair_data.tracker_distance, Game::Data::gun_types[gs.player_vector[player_id].gun_type].width-5);

                    // Update coordinates
                    crosshair_data.tracker_position.x = crosshair_data.tracker_distance*cos(gs.player_vector[player_id].gun_angle);
                    crosshair_data.tracker_position.y = -crosshair_data.tracker_distance*sin(gs.player_vector[player_id].gun_angle);
                }
                
                // Draw tracker
                DrawCircleLinesV(
                    Vector2Add(camera.transform(
                        gs.player_vector[player_id].position
                    ), crosshair_data.tracker_position),
                    crosshair_data.tracker_radius,
                    crosshair_data.circle_color
                );
                break;
            }

            case 2:
            {
                DrawCircleLinesV(
                    GetMousePosition(),
                    crosshair_data.tracker_radius,
                    crosshair_data.circle_color
                );
                break;
            }
        }
    }
}

void Pages::GameRenderer::prepare(size_t _map_idx, size_t _player_tank_type)
{
    map_index = _map_idx;
    tank_index = _player_tank_type;
    prev_times_since_last_shot.assign(12, 0);
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


void Pages::GameRenderer::CrosshairData::init()
{
    tracker_position = Vector2();
    tracker_radial_speed = 500;
    tracker_radius = 10;
    tracker_distance = 0;
}
