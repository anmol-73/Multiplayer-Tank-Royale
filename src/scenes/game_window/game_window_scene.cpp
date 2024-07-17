#include "game_window_scene.hpp"

void Scenes::GameWindowScene::update()
{
    if (WindowShouldClose()){
        SceneManagement::quit();
        return;
    }

    if (!load_done) return loading_update();

    
    // Logic update
    logic_update();


    // Drawing
    BeginDrawing();{
        ClearBackground({0, 150, 60, 255});
        draw_game();
        draw_hud();
    }
    EndDrawing();

    return;
}

void Scenes::GameWindowScene::loading_update()
{
    BeginDrawing();{
        ClearBackground(RAYWHITE);
        Scamlib::Draw::text({
            .content = "Loading",
            .position = {
                (float) GetScreenWidth() / 2,
                (float) GetScreenHeight() / 2,
            },
            .translate = {-0.5, -0.5}
        });
    }
    EndDrawing();
    return;
}

void Scenes::GameWindowScene::load_without_context()
{
    Maths::init_rand();
    // Initial starting values
    const int initial_ammo = 12;

    state = GameState::PRE_GAME;
    enemy_speed = 30.0f;
    player_speed = 120.0f;
    camera = Vector2();
    kills = 0;
    player = Vector2();
    enemies.clear();
    drops.clear();
    pregame_timer = 0;
    ammo = initial_ammo;
    ui_state.go_home_button_is_hovered = false;
    ui_state.play_again_button_is_hovered = false;

    const size_t max_enemy_count = 300;

    enemies.assign(max_enemy_count, Vector2());
    for (size_t i = 0; i < enemies.size(); ++i){
        enemies[i] = spawn_enemy();
    }
}

void Scenes::GameWindowScene::unload_without_context()
{
    enemies.clear();
    drops.clear();
}

void Scenes::GameWindowScene::logic_update()
{
    Vector2 mouse = GetMousePosition();
    
    data.offset = {((float)GetScreenWidth() / 2) - camera.x, ((float)GetScreenHeight() / 2) - camera.y};
    
    if (state == PRE_GAME){
        pregame_timer += GetFrameTime();
        return;
    }

    if (state == POST_GAME){ // We don't really have to do anything, the hud handles calling try again anol
        return;
    }
    data.gun_angle = atan2(mouse.y - player.y - data.offset.y, mouse.x - player.x - data.offset.x);
    sincos(data.gun_angle, &data.sin_gun, &data.cos_gun);


    const unsigned long long int cutoff_distance_square = 1500 * 1500;
    const float drop_attraction_distance_sqr = 200 * 200;
    const float drop_attraction_factor = 0.01;

    data.has_shot = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ammo > 0;
    if (data.has_shot) --ammo;
    float delta_time = GetFrameTime();
    
    // Handle killing enemies, moving them closer and despawning ones that are very far away
    for (size_t i = 0; i < enemies.size(); ++i){
        // Move towards player
        enemies[i] = Vector2MoveTowards(enemies[i], player, enemy_speed * delta_time);
        
        float dist_sqr = Vector2DistanceSqr(player, enemies[i]);
        
        // Check if player was killed
        if (dist_sqr < (player_radius + enemy_radius) * (player_radius + enemy_radius)){
            state = GameState::POST_GAME;
            continue;
        }

        // Check if enemy was killed
        if (
            data.has_shot &&
            Maths::segment_intersects_with_circle(
                player, {
                player.x + (float)((bullet_range) * data.cos_gun),
                player.y + (float)((bullet_range) * data.sin_gun)    
            },
            enemies[i],
            enemy_radius
        )){
            unsigned int drop_count = get_drop_amount();
            ++kills;
            for (unsigned int j = 0; j < drop_count; ++j){
                drops.push_back(
                    Vector2Add(enemies[i], {
                        (float)Maths::random(5, 20),
                        (float)Maths::random(5, 20),
                    })
                );
            }

            enemies[i] = spawn_enemy();
            continue;
        }
        
        // Check if enemy should be despawned
        if (dist_sqr > cutoff_distance_square){
            enemies[i] = spawn_enemy();
            continue;
        }
    }

    // Handle movement
    if (IsKeyDown(KEY_A)) player.x -= player_speed * delta_time;
    if (IsKeyDown(KEY_D)) player.x += player_speed * delta_time;
    if (IsKeyDown(KEY_W)) player.y -= player_speed * delta_time;
    if (IsKeyDown(KEY_S)) player.y += player_speed * delta_time;

    // Handle udpating camera
    const float max_camera_delay = 100;
    float cam_player_dist = Vector2Distance(camera, player);

    if (cam_player_dist < max_camera_delay){
        camera = Vector2MoveTowards(camera, player, (player_speed * 0.8 * delta_time));
    } else{
        camera = Vector2MoveTowards(camera, player, cam_player_dist - 100);
    }

    // Handle picking up ammo drops and despawning ones that are very far away
    for (std::list<Vector2>::iterator it = drops.begin(); it != drops.end(); ++it)
    {
        float dist_sqr = Vector2DistanceSqr(player, *it);
        if ( dist_sqr < drop_attraction_distance_sqr){
            *it = Vector2MoveTowards(*it, player, (drop_attraction_factor * (drop_attraction_distance_sqr + 2 - dist_sqr)) * delta_time);
            dist_sqr = Vector2DistanceSqr(player, *it);
        }
        
        // Check if ammo was consumed
        if (dist_sqr < (player_radius + drop_radius) * (player_radius + drop_radius)){
            ++ammo;
            drops.erase(it);
            --it;   
            continue;
        } 

        // Check if ammo is too far
        if (dist_sqr > cutoff_distance_square){
            drops.erase(it);
            --it;
            continue;
        }
    }
}


void Scenes::GameWindowScene::draw_game()
{

    // Draw enemies
    for (size_t i = 0; i < enemies.size(); ++i){
        DrawCircle(enemies[i].x + data.offset.x, enemies[i].y + data.offset.y, enemy_radius, enemy_color);
    }

    // Draw drops
    for (std::list<Vector2>::iterator it = drops.begin(); it != drops.end(); ++it){
        DrawCircle((*it).x + data.offset.x, (*it).y + data.offset.y, drop_radius, drop_color);
    }

    // Draw trce
    if (data.has_shot){
        DrawLineEx(
            {
                player.x + data.offset.x,
                player.y + data.offset.y    
            }, {
                player.x + data.offset.x + (float)((bullet_range) * data.cos_gun),
                player.y + data.offset.y + (float)((bullet_range) * data.sin_gun)    
            }, 4, RAYWHITE
        );
    }

    // Draw gun
    DrawLineEx(
        {
            player.x + data.offset.x,
            player.y + data.offset.y    
        }, {
            player.x + data.offset.x + (float)((player_radius + gun_length) * data.cos_gun),
            player.y + data.offset.y + (float)((player_radius + gun_length) * data.sin_gun)    
        }, 10, gun_color
    );

    // Draw player
    DrawCircle(player.x + data.offset.x, player.y + data.offset.y, player_radius, player_color);
}

void Scenes::GameWindowScene::draw_hud()
{
    Scamlib::Draw::text({
        .content = "FPS: " + std::to_string(GetFPS()),
        .color = DARKBROWN
    });
    Rectangle score_bounds = Scamlib::Draw::text({
        .content = std::to_string(kills),
        .position = {
            (float)GetScreenWidth() / 2,
            0
        },
        .translate = {
            -0.5, 0
        },
        .color = WHITE,
        .size = 64,
    });

    Scamlib::Draw::text({
        .content = "Ammo: " + std::to_string(ammo),
        .position = {
            (float)GetScreenWidth() / 2,
            score_bounds.y + score_bounds.height
        },
        .translate = {
            -0.5, 0
        },
        .color = BLACK,
        .size = 24,
    });

    if (state == GameState::PRE_GAME){
        if (pregame_timer > 5){
            state = GameState::GAME; return;
        }
        Scamlib::Draw::textbox(
            {
                .content = "Game Starts in " + std::to_string(5 - (int)pregame_timer),
                .position = {
                    (float)GetScreenWidth() / 2,
                    (float)GetScreenHeight() / 2
                },
                .translate = {
                    -0.5, -0.5
                },
                .color = BLACK,
                .size = 84,
            }, {
                .position = {
                    (float)GetScreenWidth()  / 2, 
                    (float)GetScreenHeight() / 2
                },
                .translate = {-0.5, -0.5},
                .fill = {100, 100, 100, 200},
                .border = {
                    .width = 2
                }
            },
            {30, 20}
        );
        return;
    }


    if (state == GameState::POST_GAME){
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        Rectangle banner_bounds = Scamlib::Draw::textbox({
                .content = "GAME OVER",
                .position = {
                    (float)GetScreenWidth() / 2,
                    (float)GetScreenHeight() / 2
                },
                .translate = {
                    -0.5, -0.5
                },
                .color = RED,
                .size = 84,
            }, {
                .position = {
                    (float)GetScreenWidth() / 2, 
                    (float)GetScreenHeight() / 2
                },
                .translate = {-0.5, -0.5},
                .fill = {100, 100, 100, 200},
                .border = {
                    .width = 2
                }
            },
            {30, 15}
        );

        Rectangle go_home_button_bounds = Scamlib::Draw::textbox(
            {
                .content = "Go Home",
                .position = {
                    ((float)GetScreenWidth()) / 2, 
                    banner_bounds.y + (float)(banner_bounds.height*1.5)
                },
                .translate = {-0.5f, -0.5f},
                .color = WHITE,
            }, {
                .position = {
                    ((float)GetScreenWidth()) / 2, 
                    banner_bounds.y + (float)(banner_bounds.height*1.5)
                },
                .translate = {-0.5, -0.5},
                .fill = ui_state.go_home_button_is_hovered? Color{152, 141, 242, 255} : Color{102, 91, 242, 255},
                .border = {
                    .width = 2
                }
            },
            {20, 15}
        );

        Rectangle play_again_button_bounds = Scamlib::Draw::textbox(
            {
                .content = "Play Again!",
                .position = {
                    ((float)GetScreenWidth()) / 2, 
                    go_home_button_bounds.y + go_home_button_bounds.height + (float)(0.5*banner_bounds.height)
                },
                .translate = {-0.5f, -0.5},
                .color = WHITE,
            }, {
                .position = {
                    (float)GetScreenWidth() / 2, 
                    go_home_button_bounds.y + go_home_button_bounds.height + (float)(0.5*banner_bounds.height)
                },
                .translate = {-0.5, -0.5},
                .fill = ui_state.play_again_button_is_hovered? Color{152, 141, 242, 255} : Color{102, 91, 242, 255},
                .border = {
                    .width = 2
                }
            },
            {20, 15}
        );
        
        if (CheckCollisionPointRec(GetMousePosition(), go_home_button_bounds)){
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            ui_state.go_home_button_is_hovered = true;
        } else{
            ui_state.go_home_button_is_hovered = false;
        }
        if (CheckCollisionPointRec(GetMousePosition(), play_again_button_bounds)){
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            ui_state.play_again_button_is_hovered = true;
        } else{
            ui_state.play_again_button_is_hovered = false;
        }

        if (ui_state.go_home_button_is_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            SceneManagement::load_scene(SceneManagement::SceneName::MAIN_WINDOW);
        }
        if (ui_state.play_again_button_is_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            SceneManagement::load_scene(SceneManagement::SceneName::GAME_WINDOW);
        }
    }
}

Vector2 Scenes::GameWindowScene::spawn_enemy()
{
    const float radius_low = 600;
    const float radius_high = 1200;

    double radius = Maths::random(radius_low, radius_high);
    double angle = Maths::random(0, 360);

    double sin, cos;
    sincos(angle, &sin, &cos);

    return {(float)(radius * cos + player.x), (float)(radius * sin + player.y)};
}

unsigned int Scenes::GameWindowScene::get_drop_amount()
{
    const float min_ammo = 0;
    const float normal_max_ammo = 2;
    const float survivor_max_ammo = 6;
    return (unsigned int)((Maths::random(min_ammo, std::max(survivor_max_ammo - ammo, normal_max_ammo))));
}
