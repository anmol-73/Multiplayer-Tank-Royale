#include "new_game_window_scene.hpp"

void Scenes::NewGameWindowScene::update()
{
    if (WindowShouldClose()){
        SceneManagement::quit();
        return;
    }

    // Loading game data
    if (context_load_now || !load_done) return loading_update();

    // Logic update
    logic_update();

    // Drawing
    BeginDrawing();{
        ClearBackground(VIOLET);
        draw_game();
        draw_hud();
    }
    EndDrawing();

    return;
}

void Scenes::NewGameWindowScene::load_with_context()
{
    player_spritesheet = LoadTextureFromImage(player_spritesheet_image);

    Utils::Animation* player_idle = new Utils::Animation(
            &player_spritesheet,
            {
                {0.3, Rectangle{.x = 0, .y = 0, .width = 24, .height = 32}},
            }
        );
    player_idle_idx = player_controller->register_animation(player_idle);

    Utils::Animation* player_moving = new Utils::Animation(
            &player_spritesheet,
            {
                {0.3, Rectangle{.x = 0, .y = 0, .width = 24, .height = 32}},
                {0.3, Rectangle{.x = 24, .y = 0, .width = 24, .height = 32}}
            }
        );
    player_moving_idx = player_controller->register_animation(player_moving);
    
    Utils::Animation* gun_idle = new Utils::Animation(
            &player_spritesheet,
            {
                {0.1, Rectangle{.x = 0, .y = 32, .width = 8, .height = 32}},
            }
        );
    gun_idle_idx = gun_controller->register_animation(gun_idle);

    Utils::Animation* gun_shot = new Utils::Animation(
            &player_spritesheet,
            {
                {0.05, Rectangle{.x = 8, .y = 32, .width = 8, .height = 32}},
                {0.05, Rectangle{.x = 16, .y = 32, .width = 8, .height = 32}},
                {0.2, Rectangle{.x = 24, .y = 32, .width = 8, .height = 32}},
                {0.05, Rectangle{.x = 32, .y = 32, .width = 8, .height = 32}},
                {0.2, Rectangle{.x = 40, .y = 32, .width = 8, .height = 32}},
                {0.1, Rectangle{.x = 48, .y = 32, .width = 8, .height = 32}},
            }
        );
    gun_shot_idx = gun_controller->register_animation(gun_shot);
}

void Scenes::NewGameWindowScene::loading_update()
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

void Scenes::NewGameWindowScene::load_without_context()
{
    Maths::init_rand();
    // Initial starting values
    const int initial_ammo = 12;

    state = GameState::PRE_GAME;
    SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
    enemy_speed = 30.0f;
    player_data.player_speed = 120.0f;
    player_data.player_angle = 0;
    gun_data.gun_angle=0;
    camera = Vector2();
    kills = 0;
    player_position = Vector2();
    crosshair_data.tracker_position = Vector2();
    set_position();
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

    player_spritesheet_image = LoadImage("resources/new_game_window/tank2_spritesheet.png");
    player_controller = new Utils::AnimationController();
    gun_controller = new Utils::AnimationController();
    main_load_ready = true;
}

void Scenes::NewGameWindowScene::unload_without_context()
{
    UnloadImage(player_spritesheet_image);
    enemies.clear();
    drops.clear();
    delete player_controller;    
    delete gun_controller;
}

void Scenes::NewGameWindowScene::unload_with_context()
{
    UnloadTexture(player_spritesheet);
}

void Scenes::NewGameWindowScene::logic_update()
{
    mouse = GetMousePosition();
    offset = {((float)GetScreenWidth() / 2) - camera.x, ((float)GetScreenHeight() / 2) - camera.y};
    set_position();
    
    if (state == PRE_GAME){
        pregame_timer += GetFrameTime();
        return;
    }

    if (state == POST_GAME){ // We don't really have to do anything, the hud handles calling try again anol
        return;
    }

    float delta_time = GetFrameTime();

    // Handle movement of gun
    set_gun_angle(delta_time);
    
    sincos(gun_data.gun_angle, &gun_data.sin_gun, &gun_data.cos_gun);
    sincos(player_data.player_angle, &player_data.sin_player, &player_data.cos_player);

    // Handle movement of tracker circle
    set_tracker(delta_time);

    const unsigned long long int cutoff_distance_square = 1500 * 1500;
    const float drop_attraction_distance_sqr = 200 * 200;
    const float drop_attraction_factor = 0.01;

    gun_data.has_shot = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ammo > 0;
    if (gun_data.has_shot) --ammo;
    
    // Handle killing enemies, moving them closer and despawning ones that are very far away
    for (size_t i = 0; i < enemies.size(); ++i){
        // Move towards player
        enemies[i] = Vector2MoveTowards(enemies[i], player_position, enemy_speed * delta_time);
        
        float dist_sqr = Vector2DistanceSqr(player_position, enemies[i]);
        
        // Check if player was killed
        if (dist_sqr < (player_radius + enemy_radius) * (player_radius + enemy_radius)){
            // state = GameState::POST_GAME;
            continue;
        }

        // Check if enemy was killed
        if (
            gun_data.has_shot &&
            Maths::segment_intersects_with_circle(
                player_position, {
                player_position.x + (float)((bullet_range) * gun_data.cos_gun),
                player_position.y + (float)((bullet_range) * gun_data.sin_gun)    
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
    handle_movement(delta_time);  

    // Handle udpating camera
    const float max_camera_delay = 100;
    float cam_player_dist = Vector2Distance(camera, player_position);

    if (cam_player_dist < max_camera_delay){
        camera = Vector2MoveTowards(camera, player_position, (player_data.player_speed * 0.8 * delta_time));
    } else{
        camera = Vector2MoveTowards(camera, player_position, cam_player_dist - 100);
    }

    // Handle picking up ammo drops and despawning ones that are very far away
    for (std::list<Vector2>::iterator it = drops.begin(); it != drops.end(); ++it)
    {
        float dist_sqr = Vector2DistanceSqr(player_position, *it);
        if ( dist_sqr < drop_attraction_distance_sqr){
            *it = Vector2MoveTowards(*it, player_position, (drop_attraction_factor * (drop_attraction_distance_sqr + 2 - dist_sqr)) * delta_time);
            dist_sqr = Vector2DistanceSqr(player_position, *it);
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

    // Handle gun animation
    if(gun_data.has_shot)
    {
        gun_controller->play(gun_shot_idx, true);
    }
    else if(gun_controller->current_iteration_count>0)
    {
        gun_controller->play(gun_idle_idx, true);
    }
}


void Scenes::NewGameWindowScene::draw_game()
{
    float delta_time = GetFrameTime();
    player_controller->update(delta_time);
    gun_controller->update(delta_time);

    // Draw enemies
    for (size_t i = 0; i < enemies.size(); ++i){
        DrawCircle(enemies[i].x + offset.x, enemies[i].y + offset.y, enemy_radius, enemy_color);
    }

    // Draw drops
    for (std::list<Vector2>::iterator it = drops.begin(); it != drops.end(); ++it){
        DrawCircle((*it).x + offset.x, (*it).y + offset.y, drop_radius, drop_color);
    }

    // Draw trace
    if (gun_data.has_shot){
        DrawLineEx(
            {
                player_position.x + offset.x,
                player_position.y + offset.y    
            }, {
                player_position.x + offset.x + (float)((bullet_range) * gun_data.cos_gun),
                player_position.y + offset.y + (float)((bullet_range) * gun_data.sin_gun)    
            }, 4, RAYWHITE
        );
    }

    // Draw player
    // DrawRectanglePro(player_data.player, 
    //     {player_data.player.width/2, player_data.player.height/2},
    //     -player_data.player_angle*RAD2DEG,
    //     player_color
    // );

    // Draw player
    Texture* player_texture = player_controller->get_sprite().first;
    Rectangle* player_source = player_controller->get_sprite().second;
    DrawTexturePro(*player_texture,
    *player_source,
    player_data.player,
    {player_data.player.width/2, player_data.player.height/2},
    -(player_data.player_angle - PI/2)*RAD2DEG, WHITE);
    

    // Draw gun
    Texture* gun_texture = gun_controller->get_sprite().first;
    Rectangle* gun_source = gun_controller->get_sprite().second;
    DrawTexturePro(*gun_texture,
        *gun_source,
        gun_data.gun,
        {
            gun_data.gun.width/2,
            gun_data.gun.height
        }, 
        (gun_data.gun_angle+PI/2)*RAD2DEG, WHITE
    );

    // Draw gun crosshair circle
    DrawCircleLines(crosshair_data.tracker_position.x+player_position.x+offset.x, 
        crosshair_data.tracker_position.y+player_position.y+offset.y,
        crosshair_data.tracker_radius,
        crosshair_data.circle_color);
}

void Scenes::NewGameWindowScene::draw_hud()
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
            SceneManagement::load_scene(SceneManagement::SceneName::NEW_GAME_WINDOW);
        }
    }
}


void Scenes::NewGameWindowScene::set_position()
{
    player_data.player.x = player_position.x + offset.x;
    player_data.player.y = player_position.y + offset.y;
    gun_data.gun.x = player_position.x + offset.x;
    gun_data.gun.y = player_position.y + offset.y;
}

void Scenes::NewGameWindowScene::handle_movement(float delta_time)
{
    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
    {
        player_controller->play(player_moving_idx, false);
        if (IsKeyDown(KEY_A)) player_data.player_angle += player_data.player_rot_speed * delta_time;
        if (IsKeyDown(KEY_D)) player_data.player_angle -= player_data.player_rot_speed * delta_time;

        if (IsKeyDown(KEY_W))
        {
            double displacement = (player_data.player_speed * delta_time);
            player_position.x += (displacement*player_data.cos_player);
            player_position.y -= (displacement*player_data.sin_player);
        }
        if (IsKeyDown(KEY_S))
        {
            double displacement = (player_data.player_speed * delta_time);
            player_position.x -= (displacement*player_data.cos_player);
            player_position.y += (displacement*player_data.sin_player);
        }
        set_position();
    }
    else
    {
        player_controller->play(player_idle_idx);
    }
}

double Scenes::NewGameWindowScene::normalize_angle(double angle)
{
    while (angle >= 2 * PI) angle -= 2.0 * PI;
    while (angle < 0) angle += 2.0 * PI;
    return angle;
}

void Scenes::NewGameWindowScene::set_gun_angle(float delta_time)
{
    gun_data.predicted_gun_angle = normalize_angle(atan2(mouse.y - player_position.y - offset.y, mouse.x - player_position.x - offset.x));
    double angle_difference = normalize_angle(gun_data.predicted_gun_angle - gun_data.gun_angle);
    
    // Take shortest path to targeted gun_angle
    {
        double max_angular_displacement = (gun_data.gun_rot_speed * delta_time);
        if (angle_difference < PI)
        {
            gun_data.gun_angle += max_angular_displacement;
            // Stop jittering by preventing overshoot
            if (gun_data.gun_angle > gun_data.predicted_gun_angle && gun_data.gun_angle - gun_data.predicted_gun_angle < PI){
                gun_data.gun_angle = gun_data.predicted_gun_angle;
            }
            
        }
        else
        {
            gun_data.gun_angle -= max_angular_displacement;
            // Stop jittering by preventing overshoot
            if (gun_data.gun_angle < gun_data.predicted_gun_angle && gun_data.predicted_gun_angle - gun_data.gun_angle < PI){
                gun_data.gun_angle = gun_data.predicted_gun_angle;
            }
        }

    }
    gun_data.gun_angle = normalize_angle(gun_data.gun_angle);
}

void Scenes::NewGameWindowScene::set_tracker(float delta_time)
{
    // Targeted radial distance (mouse distance)
    crosshair_data.mouse_distance = std::sqrt(((mouse.x-player_position.x-offset.x)*(mouse.x-player_position.x-offset.x)) + ((mouse.y-player_position.y-offset.y)*(mouse.y-player_position.y-offset.y)));
    
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
    crosshair_data.tracker_distance = std::max((float)crosshair_data.tracker_distance, gun_data.gun.width + 5);

    // Update coordinates
    crosshair_data.tracker_position.x = crosshair_data.tracker_distance*gun_data.cos_gun;
    crosshair_data.tracker_position.y = crosshair_data.tracker_distance*gun_data.sin_gun;
}

Vector2 Scenes::NewGameWindowScene::spawn_enemy()
{
    const float radius_low = 600;
    const float radius_high = 1200;

    double radius = Maths::random(radius_low, radius_high);
    double angle = Maths::random(0, 360);

    double sin, cos;
    sincos(angle, &sin, &cos);

    return {(float)(radius * cos + player_position.x), (float)(radius * sin + player_position.y)};
}

unsigned int Scenes::NewGameWindowScene::get_drop_amount()
{
    const float min_ammo = 0;
    const float normal_max_ammo = 2;
    const float survivor_max_ammo = 6;
    return (unsigned int)((Maths::random(min_ammo, std::max(survivor_max_ammo - ammo, normal_max_ammo))));
}

/**
 * TODO!
 * Allow for a load with context and unload with context (Cause you cant do gpu shit ig) (DONE)
 * Add some rudimentary layout management to scamlib
 * Cleanup code to make it look nice :) (WE'LL KEEP DOING)
 * Also move AnimationController definitions to cpp file (DONE)
 * Also do runtime check of Animation data (DONE)
 * 
 * Add some like event handling business in scamlib
 * 
 * Animation speed? With keyframes as percentages (LITE)
 * 
 * 
 */