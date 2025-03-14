#include "game_renderer.hpp"
#include <iostream>
#include <algorithm>
#include <stdlib.h>

Pages::GameRenderer::GameRenderer()
{
    map_images.assign(Maps::map_count, {});
    for (size_t i = 0; i < Maps::map_count; ++i){
        map_images[i].path = Maps::maps[i].resource_path;
    }
    
    tank_spritesheets.assign(Game::Data::tank_resources.size(), {});
    for (size_t i = 0; i < tank_spritesheets.size(); ++i){
        tank_spritesheets[i].path = Game::Data::tank_resources[i][2];
    }
    
    gun_spritesheets.assign(Game::Data::gun_resources.size(), {});
    for (size_t i = 0; i < gun_spritesheets.size(); ++i){
        gun_spritesheets[i].path = Game::Data::gun_resources[i][2];
    }

    explosion_spritesheet.path = "resources/game_window/misc/ONLY_EXPLOSION.png";
    skull_spritesheet.path = "resources/game_window/misc/TARGET.png";
    projectile_spritesheet.path = "resources/game_window/misc/PROJECTILE.png";
    spacebg_spritesheet.path = "resources/game_window/spacebg.png";
    
}

void Pages::GameRenderer::draw(const Game::GameState& gs, int player_id, const std::vector<Communication::Game::PlayerIdentification>& pd)
{
    auto delta_time = GetFrameTime();
    if (gs.player_vector[player_id].is_alive){
        SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
    }

    if(IsKeyPressed(KEY_P)){            
        Global::Rendering::Shaders::rendering_shader = LoadShader(0, TextFormat("resources/shaders/postproc.fs"));
    }
    BeginShaderMode(Global::Rendering::Shaders::rendering_shader);{
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
        effect_ac.draw(delta_time);
        { // draw tanks
            for (size_t i = 0; i < gs.player_vector.size(); ++ i){
                if (!(gs.player_vector[i].exists and gs.player_vector[i].is_alive)) continue;
                tank_acs[i].play(gs.player_vector[i].tank_type * 2 + gs.player_vector[i].is_moving, false);
                
                auto rect = camera.transform(
                    {
                        gs.player_vector[i].position.x, gs.player_vector[i].position.y,
                        static_cast<float>(Game::Data::tank_types[gs.player_vector[i].tank_type].width), static_cast<float>(Game::Data::tank_types[gs.player_vector[i].tank_type].height)
                    }
                );
                tank_acs[i].draw(delta_time, rect, gs.player_vector[i].angle);
                // DrawRectanglePro(
                    //     camera.transform(
                //         {
                //             gs.player_vector[i].position.x, gs.player_vector[i].position.y,
                //             static_cast<float>(Game::Data::tank_types[gs.player_vector[i].tank_type].width), static_cast<float>(Game::Data::tank_types[gs.player_vector[i].tank_type].height)
                //         }
                //     ), camera.scale(Vector2{
                    //         static_cast<float>(Game::Data::tank_types[gs.player_vector[i].tank_type].width/2), static_cast<float>(Game::Data::tank_types[gs.player_vector[i].tank_type].height/2)
                    //     }), -RAD2DEG*gs.player_vector[i].angle, WHITE
                    // );
                }
            }
            
        { // draw gun
            for (size_t i = 0; i < gs.player_vector.size(); ++ i)
            {
                if((!gs.player_vector[i].exists) || (!gs.player_vector[i].is_alive)) continue;
                
                bool has_shot = gs.player_vector[i].time_since_last_shot < prev_times_since_last_shot[i];
                if (gun_acs[i].current_anim % 2 == 0 or gun_acs[i].current_iteration_count > 0){
                    gun_acs[i].play(gs.player_vector[i].gun_type * 2 + has_shot, false);

                    if (has_shot){
                        if (gs.player_vector[i].gun_type == 2){
                            Rectangle rect = {
                                gs.player_vector[i].ray_contact.x, gs.player_vector[i].ray_contact.y, static_cast<float>(Game::Data::explosion_types[0].radius) * 2, static_cast<float>(Game::Data::explosion_types[0].radius) * 2
                            };
                            effect_ac.play(0, [rect, this](float _){
                                return std::pair<Rectangle, float>{camera.transform(rect), 0};
                            });
                        }
                    }
                }
                switch (gs.player_vector[i].gun_type)
                {
                    case 0: // Gun behaviour
                    {   // Draw traces
                        if(gs.player_vector[i].time_since_last_shot <= 0.07)
                        {
                            DrawLineEx(
                                camera.transform(gs.player_vector[i].position),
                                camera.transform(gs.player_vector[i].ray_contact),
                                4, GREEN
                            );
                        }
                    }

                    case 1:
                    case 2:
                    {   // Draw Gun
                        auto rect = camera.transform(
                            {
                                gs.player_vector[i].position.x, gs.player_vector[i].position.y,
                                static_cast<float>(Game::Data::gun_types[gs.player_vector[i].tank_type].width), static_cast<float>(Game::Data::gun_types[gs.player_vector[i].tank_type].height)
                            }
                        );
                        gun_acs[i].draw(delta_time, rect, gs.player_vector[i].gun_angle);
                        break;
                    }
                }
                prev_times_since_last_shot[i] = gs.player_vector[i].time_since_last_shot;
            }
        }

        
        { // draw projectiles
            for (auto it = gs.projectile_vector.begin(); it != gs.projectile_vector.end(); ++it){
                DrawTexturePro(
                    projectile_spritesheet.tex,
                    {0,0,36,16},
                    camera.transform({
                        (*it).position.x, (*it).position.y,
                        static_cast<float>(Game::Data::projectile_types[(*it).type].width), static_cast<float>(Game::Data::projectile_types[(*it).type].height)
                    }),
                    camera.scale(Vector2{
                        static_cast<float>(Game::Data::projectile_types[(*it).type].width/2), static_cast<float>(Game::Data::projectile_types[(*it).type].height/2)
                    }), 
                    -(*it).angle*RAD2DEG, WHITE
                );
            }
        }


        // { // draw explosions
        //     for (auto it = gs.explosion_vector.begin(); it != gs.explosion_vector.end(); ++it){
                
        //         auto position = camera.transform((*it).position);
        //         DrawCircleGradient(
        //             position.x, position.y,
        //             camera.scale(Game::Data::explosion_types[(*it).type].radius), 
        //             MAGENTA, RED
        //         );
        //     }
        // }
        }
    EndShaderMode();
    { // draw health bar and name
        for (size_t j = 0; j < pd.size(); ++j)
        {
            size_t i = pd[j].id;
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
            
            DragonLib::Utils::Drawing::draw_text({
                .content = pd[j].name,
                .sdf=false,
                .font_size = Global::rem * 0.5f,
                .font_color = BLACK,
                .position = {.value = {health_bar.x + actual_width/2, health_bar.y + health_bar.height/2}, .mode = {DragonLib::UI::DrawParameters::ABSOLUTE, DragonLib::UI::DrawParameters::ABSOLUTE}},
            });

            float reloadfactor = std::min(static_cast<float>(gs.player_vector[i].time_since_last_shot)/static_cast<float>(Game::Data::gun_types[gs.player_vector[i].gun_type].reload_time), static_cast<float>(1));

            Rectangle reload_bar = {
                .x = gs.player_vector[i].position.x,
                .y = gs.player_vector[i].position.y,
                .width = 100,
                .height = 10
            };

            reload_bar = camera.transform(reload_bar);
            Vector2 rboffset = camera.scale(Vector2{-50, -static_cast<float>(Game::Data::tank_types[gs.player_vector[i].tank_type].height)-10});
            
            reload_bar.x += rboffset.x;
            reload_bar.y += rboffset.y;
            DrawRectangleRounded(reload_bar, 1.0, 6, GRAY);

            float rb_actual_width = reload_bar.width;

            reload_bar.width *= reloadfactor; 
            DrawRectangleRounded(reload_bar, 5, 6, BLUE);
            
            DragonLib::Utils::Drawing::draw_text({
                .content = "WEAPON READY",
                .sdf=false,
                .font_size = Global::rem * 0.5f,
                .font_color = BLACK,
                .position = {.value = {reload_bar.x + rb_actual_width/2, reload_bar.y + reload_bar.height/2}, .mode = {DragonLib::UI::DrawParameters::ABSOLUTE, DragonLib::UI::DrawParameters::ABSOLUTE}},
            });

        }
    }

    if (gs.player_vector[player_id].is_alive){ // Draw tracker
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
                    crosshair_data.tracker_distance = std::max(crosshair_data.tracker_distance, Game::Data::gun_types[gs.player_vector[player_id].gun_type].width/2-20);

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
    camera.init({Maps::maps[map_index].width(), Maps::maps[map_index].height()}, {Maps::maps[map_index].vwidth(), Maps::maps[map_index].vheight()});
}

void Pages::GameRenderer::load_async()
{
    for (size_t i = 0; i < Maps::map_count; ++i){
        map_images[i].load_im();
    }
    for (size_t i = 0; i < tank_spritesheets.size(); ++i){
        tank_spritesheets[i].load_im();
    }
    for (size_t i = 0; i < gun_spritesheets.size(); ++i){
        gun_spritesheets[i].load_im();
    }

    explosion_spritesheet.load_im();
    skull_spritesheet.load_im();
    projectile_spritesheet.load_im();
    spacebg_spritesheet.load_im();

    tank_acs.reserve(12);
    const float tank_anim_duration = 0.4;
    const int tank_anim_frame_count = 2;
    for (size_t i = 0; i < 12; ++i){
        tank_acs.emplace_back();
        for (size_t tank_type = 0; tank_type < tank_spritesheets.size(); ++tank_type){
            tank_acs[i].register_animation(
                new Utils::AAnimation(
                    tank_anim_duration,
                    [tank_type, this, tank_anim_frame_count](float time, Rectangle rect, float angle){
                        int height = tank_spritesheets[tank_type].tex.height / tank_anim_frame_count;
                        DrawTexturePro(
                            tank_spritesheets[tank_type].tex,
                            {
                                .x = 0,
                                .y = 0,
                                .width = static_cast<float>(tank_spritesheets[tank_type].tex.width),
                                .height = static_cast<float>(height)
                            },
                            rect,
                            {rect.width / 2, rect.height / 2}, -RAD2DEG*angle, WHITE
                        );
                    }
                )
            );
            tank_acs[i].register_animation(
                new Utils::AAnimation(
                    tank_anim_duration,
                    [tank_type, this, tank_anim_frame_count, tank_anim_duration](float time, Rectangle rect, float angle){
                        int frame = time * tank_anim_frame_count / tank_anim_duration;
                        frame = std::min(frame, tank_anim_frame_count - 1);
                        int height = tank_spritesheets[tank_type].tex.height / tank_anim_frame_count;
                        DrawTexturePro(
                            tank_spritesheets[tank_type].tex,
                            {
                                .x = 0,
                                .y = static_cast<float>(frame * height),
                                .width = static_cast<float>(tank_spritesheets[tank_type].tex.width),
                                .height = static_cast<float>(height)
                            },
                            rect,
                            {rect.width / 2, rect.height / 2}, -RAD2DEG*angle, WHITE
                        );
                    }
                )
            );
        }
    }
    // Do the same scene for gun animations!
    gun_acs.reserve(12);
    for (size_t i = 0; i < 12; ++i){
        gun_acs.emplace_back();
        for (size_t gun_type = 0; gun_type < gun_spritesheets.size(); ++gun_type){
            float gun_anim_duration;
            if(gun_type == 0)
            {
                gun_anim_duration = 0.2;
            }
            else if(gun_type == 1)
            {
                gun_anim_duration = 0.5;
            }
            else
            {
                gun_anim_duration = 0.5;
            }
            int gun_animation_frame_count = gun_type > 1 ? 6 : 4;
            gun_acs[i].register_animation(
                new Utils::AAnimation(
                    gun_anim_duration,
                    [gun_type, this, gun_animation_frame_count](float time, Rectangle rect, float angle){
                        int height = gun_spritesheets[gun_type].tex.height / gun_animation_frame_count;
                        DrawTexturePro(
                            gun_spritesheets[gun_type].tex,
                            {
                                .x = 0,
                                .y = 0,
                                .width = static_cast<float>(gun_spritesheets[gun_type].tex.width),
                                .height = static_cast<float>(height)
                            },
                            rect,
                            {rect.width / 2, rect.height / 2}, -RAD2DEG*angle, WHITE
                        );
                    }
                )
            );
            gun_acs[i].register_animation(
                new Utils::AAnimation(
                    gun_anim_duration,
                    [gun_type, this, gun_animation_frame_count, gun_anim_duration](float time, Rectangle rect, float angle){
                        int frame = time * gun_animation_frame_count / gun_anim_duration;
                        frame = std::min(frame, gun_animation_frame_count - 1);
                        int height = gun_spritesheets[gun_type].tex.height / gun_animation_frame_count;
                        DrawTexturePro(
                            gun_spritesheets[gun_type].tex,
                            {
                                .x = 0,
                                .y = static_cast<float>(frame * height),
                                .width = static_cast<float>(gun_spritesheets[gun_type].tex.width),
                                .height = static_cast<float>(height)
                            },
                            rect,
                            {rect.width / 2, rect.height / 2}, -RAD2DEG*angle, WHITE
                        );
                    }
                )
            );
        }
    }

    effect_ac.register_animation(
        new Utils::AAnimation(
            2.3, [this](float time, Rectangle rect, float _angle){
                std::cout << time << rect.x << ' ' << rect.y << std::endl;
                if (time < 1.5){
                    float width = std::min((time / 0.5f) * rect.width, rect.width);
                    
                    DrawRing(
                        {rect.x, rect.y}, rect.width / 2 - 4, rect.width / 2, 0, 2 * RAD2DEG * PI, 24, {230, 10, 10, 200}
                    );
                    // DrawCircleLines(
                    //     rect.x, rect.y, rect.width / 2, {230, 10, 10, 200}
                    // );
                    rect.width = width; rect.height = width;
                    DrawTexturePro(
                        skull_spritesheet.tex,
                        {
                            .x = 0,
                            .y = 0,
                            .width = static_cast<float>(skull_spritesheet.tex.width),
                            .height = static_cast<float>(skull_spritesheet.tex.height)
                        },
                        rect,
                        {rect.width / 2, rect.height / 2}, 0, {255, 255, 255, 200}
                    );
                    return;
                }
                if (time < 2){
                    float ratio = ((time - 1.5)/ 0.5);
                    float width = (1 - ratio) * rect.width;
                    DrawRing(
                        {rect.x, rect.y}, rect.width / 2 - 4, rect.width / 2, 0, 2 * PI * RAD2DEG, 24, {230, 10, 10, 200}
                    );
                    rect.width = width; rect.height = width;
                    DrawTexturePro(
                        skull_spritesheet.tex,
                        {
                            .x = 0,
                            .y = 0,
                            .width = static_cast<float>(skull_spritesheet.tex.width),
                            .height = static_cast<float>(skull_spritesheet.tex.height)
                        },
                        rect,
                        {rect.width / 2, rect.height / 2}, PI * 2 * RAD2DEG * ratio, {255, 255, 255, 200}
                    );
                    return;
                }
                int frame_count = 8;

                int frame = (time - 2) * frame_count / 0.3f;
                frame = std::min(frame, frame_count - 1);
                int width = explosion_spritesheet.tex.width / frame_count;
                DrawTexturePro(
                    explosion_spritesheet.tex,
                    {
                        .x = static_cast<float>(frame * width),
                        .y = 0,
                        .width = static_cast<float>(width),
                        .height = static_cast<float>(explosion_spritesheet.tex.height)
                    },
                    rect,
                    {rect.width / 2, rect.height / 2}, 0, WHITE
                );

            }
        )
    );
}

void Pages::GameRenderer::load_sync()
{
    for (size_t i = 0; i < Maps::map_count; ++i){
        map_images[i].load_tex();
    }
    for (size_t i = 0; i < tank_spritesheets.size(); ++i){
        tank_spritesheets[i].load_tex();
    }
    for (size_t i = 0; i < gun_spritesheets.size(); ++i){
        gun_spritesheets[i].load_tex();
    }

    skull_spritesheet.load_tex();
    explosion_spritesheet.load_tex();
    spacebg_spritesheet.load_tex();

}

void Pages::GameRenderer::cleanup_async()
{
    tank_acs.clear();
    gun_acs.clear();
    for (size_t i = 0; i < Maps::map_count; ++i){
        map_images[i].unload_im();
    }
    for (size_t i = 0; i < tank_spritesheets.size(); ++i){
        tank_spritesheets[i].unload_im();
    }
    for (size_t i = 0; i < gun_spritesheets.size(); ++i){
        gun_spritesheets[i].unload_im();
    }
    skull_spritesheet.unload_im();
    explosion_spritesheet.unload_im();
    spacebg_spritesheet.unload_im();
}

void Pages::GameRenderer::cleanup_sync()
{
    for (size_t i = 0; i < Maps::map_count; ++i){
        map_images[i].unload_tex();
    }
    for (size_t i = 0; i < tank_spritesheets.size(); ++i){
        tank_spritesheets[i].unload_tex();
    }
    for (size_t i = 0; i < gun_spritesheets.size(); ++i){
        gun_spritesheets[i].unload_tex();
    }
    skull_spritesheet.unload_tex();
    explosion_spritesheet.unload_tex();
    spacebg_spritesheet.unload_tex();
}


void Pages::GameRenderer::CrosshairData::init()
{
    tracker_position = Vector2();
    tracker_radial_speed = 500;
    tracker_radius = 10;
    tracker_distance = 0;
}

void Pages::GameRenderer::draw_leaderboard(const Game::GameState& gs, const std::vector<Communication::Game::PlayerIdentification>& pd)
{
    int screenWidth = GetScreenWidth();
    // int screenHeight = GetScreenHeight();

    int leaderboardWidth = 220;
    int leaderboardHeight = 250;
    int padding = 15;
    int xPos = screenWidth - leaderboardWidth - padding; 
    int yPos = padding;

    DrawRectangle(xPos, yPos, leaderboardWidth, leaderboardHeight, {0x45, 0x41, 0x39, 0xf0});

    DrawText("Leaderboard", xPos + 10, yPos + 10, 22, RAYWHITE);

    std::vector<std::pair<std::string, int>> leaderboard;
    for (size_t i = 0; i < pd.size(); i++) {
        if (!gs.player_vector[pd[i].id].exists) continue;
        leaderboard.push_back(std::make_pair(pd[i].name, gs.player_vector[pd[i].id].score));
    }
    std::sort(leaderboard.begin(), leaderboard.end(), [](const auto& a, const auto& b) { return a.second > b.second;});

   
    int textY = yPos + 40;
    for (const auto& player : leaderboard) {
        std::string playerText = player.first + " - " + std::to_string(player.second);
        DrawText(playerText.c_str(), xPos + 10, textY, 18, RAYWHITE);
        textY += 30; 
    }
}

