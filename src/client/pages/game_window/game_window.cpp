#include "game_window.hpp"

void Pages::GameWindowScene::_update()
{
    
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

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

void Pages::GameWindowScene::_load_with_context()
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

    floor_tileset = LoadTextureFromImage(floor_tileset_image);
}

void Pages::GameWindowScene::_loading_update()
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

void Pages::GameWindowScene::_load()
{
    Maths::init_rand();

    SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

    player_data.player_speed = 120.0f;
    player_data.player_angle = PI;
    gun_data.gun_angle=0;
    camera = Vector2();

    player_position = Vector2();
    crosshair_data.tracker_position = Vector2();
    set_position();

    player_spritesheet_image = LoadImage("resources/game_window/tank2_spritesheet.png");
    player_controller = new Utils::AnimationController();
    gun_controller = new Utils::AnimationController();
    main_load_ready = true;

    floor_tileset_image = LoadImage("resources/game_window/floor_tileset.png");
    wall_data.width = Maths::gcd(GetScreenHeight(),GetScreenWidth());
    wall_data.num_x = GetScreenWidth()/wall_data.width;
    wall_data.num_y = GetScreenHeight()/wall_data.width;
}

void Pages::GameWindowScene::_cleanup()
{
    UnloadImage(player_spritesheet_image);
    UnloadImage(floor_tileset_image);
    delete player_controller;
    delete gun_controller;
}

void Pages::GameWindowScene::_cleanup_with_context()
{
    UnloadTexture(player_spritesheet);
    UnloadTexture(floor_tileset);
}

void Pages::GameWindowScene::logic_update()
{
    mouse = GetMousePosition();
    offset = {((float)GetScreenWidth() / 2) - camera.x, ((float)GetScreenHeight() / 2) - camera.y};
    set_position();

    float delta_time = GetFrameTime();

    // Handle movement of gun
    set_gun_angle(delta_time);
    
    sincos(gun_data.gun_angle, &gun_data.sin_gun, &gun_data.cos_gun);
    sincos(player_data.player_angle, &player_data.sin_player, &player_data.cos_player);

    // Handle movement of tracker circle
    set_tracker(delta_time);

    gun_data.has_shot = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ammo > 0;
    if (gun_data.has_shot) ++ammo;

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

    // Handle gun animation
    if(gun_data.has_shot)
    {
        gun_controller->play(gun_shot_idx, true);
    }
    else if(gun_controller->current_iteration_count>0)
    {
        gun_controller->play(gun_idle_idx, true);
    }

    // Handle collision detection
    player_data.player_colliding=false;
    for(int i = 0; i<wall_data.num_y; i++)
    {
        for(int j = 0; j<wall_data.num_x; j++)
        {
            if(std::find(wall_data.walls.begin(), wall_data.walls.end(), std::make_pair(i,j)) != wall_data.walls.end())
            {
                Rectangle wall = {
                    .x = player_position.x+(j*wall_data.width),
                    .y = player_position.y+(i*wall_data.width),
                    .width = wall_data.width,
                    .height = wall_data.width
                };
                player_data.player_colliding = Physics::sat_collision_detection(wall, 0, player_data.player, player_data.player_angle);
                if(player_data.player_colliding){break;}
            }
            if(player_data.player_colliding){break;}
        }
        if(player_data.player_colliding){break;}
    }
}



void Pages::GameWindowScene::draw_game()
{
    float delta_time = GetFrameTime();
    player_controller->update(delta_time);
    gun_controller->update(delta_time);

    // Draw floor
    draw_bg();

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
    Texture* player_texture = player_controller->get_sprite().first;
    Rectangle* player_source = player_controller->get_sprite().second;
    DrawTexturePro(*player_texture,
    *player_source,
    player_data.player,
    {player_data.player.width/2, player_data.player.height/2},
    -(player_data.player_angle - PI/2)*RAD2DEG, WHITE
    );
    
    // Draw gun
    Texture* gun_texture = gun_controller->get_sprite().first;
    Rectangle* gun_source = gun_controller->get_sprite().second;
    Color tank_color = player_data.player_colliding ? RED:WHITE;
    DrawTexturePro(*gun_texture,
        *gun_source,
        gun_data.gun,
        {
            gun_data.gun.width/2,
            gun_data.gun.height
        }, 
        (gun_data.gun_angle+PI/2)*RAD2DEG, tank_color
    );

    // Draw gun crosshair circle
    DrawCircleLines(crosshair_data.tracker_position.x+player_position.x+offset.x, 
        crosshair_data.tracker_position.y+player_position.y+offset.y,
        crosshair_data.tracker_radius,
        crosshair_data.circle_color
        );
}

void Pages::GameWindowScene::draw_hud()
{
    Scamlib::Draw::text({
        .content = "FPS: " + std::to_string(GetFPS()),
        .color = DARKBROWN
    });
}

void Pages::GameWindowScene::draw_bg()
{
    for(int i = 0; i<wall_data.num_y; i++)
    {
        for(int j = 0; j<wall_data.num_x; j++)
        {
            Rectangle tile;
            Rectangle wall = {
                .x = player_position.x+(j*wall_data.width),
                .y = player_position.y+(i*wall_data.width),
                .width = wall_data.width,
                .height = wall_data.width
            };
            Vector2 center = {(wall_data.width/2), (wall_data.width/2)};
            if(std::find(wall_data.walls.begin(), wall_data.walls.end(), std::make_pair(i,j)) != wall_data.walls.end())
            {
                tile = {
                    .x = 16,
                    .y = 0,
                    .width = 16,
                    .height = 16,
                };
            }
            else
            {
                tile = {
                    .x = 32,
                    .y = 0,
                    .width = 16,
                    .height = 16,
                };
            }
            DrawTexturePro(floor_tileset, tile, wall, center, 0, WHITE);
        }
    }
}

void Pages::GameWindowScene::set_position()
{
    player_data.player.x = player_position.x + offset.x;
    player_data.player.y = player_position.y + offset.y;
    gun_data.gun.x = player_position.x + offset.x;
    gun_data.gun.y = player_position.y + offset.y;
}

void Pages::GameWindowScene::handle_movement(float delta_time)
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

double Pages::GameWindowScene::normalize_angle(double angle)
{
    while (angle >= 2 * PI) angle -= 2.0 * PI;
    while (angle < 0) angle += 2.0 * PI;
    return angle;
}

void Pages::GameWindowScene::set_gun_angle(float delta_time)
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

void Pages::GameWindowScene::set_tracker(float delta_time)
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
    crosshair_data.tracker_distance = std::max((float)crosshair_data.tracker_distance, gun_data.gun.height-5);

    // Update coordinates
    crosshair_data.tracker_position.x = crosshair_data.tracker_distance*gun_data.cos_gun;
    crosshair_data.tracker_position.y = crosshair_data.tracker_distance*gun_data.sin_gun;
}