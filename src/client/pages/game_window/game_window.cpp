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
        ClearBackground(SKYBLUE);
        draw_game();
        draw_hud();
    }
    EndDrawing();

    return;
}

void Pages::GameWindowScene::_load_with_context()
{
    LogicUtils::player_spritesheet = LoadTextureFromImage(LogicUtils::player_spritesheet_image);

    Utils::Animation* player_idle = new Utils::Animation(
            &LogicUtils::player_spritesheet,
            {
                {0.3, Rectangle{.x = 32, .y = 0, .width = 32, .height = 24}},
            }
        );
    LogicUtils::player_idle_idx = LogicUtils::player_controller->register_animation(player_idle);

    Utils::Animation* player_moving = new Utils::Animation(
            &LogicUtils::player_spritesheet,
            {
                {0.3, Rectangle{.x = 32, .y = 0, .width = 32, .height = 24}},
                {0.3, Rectangle{.x = 32, .y = 24, .width = 32, .height = 24}}
            }
        );
    LogicUtils::player_moving_idx = LogicUtils::player_controller->register_animation(player_moving);
    
    Utils::Animation* gun_idle = new Utils::Animation(
            &LogicUtils::player_spritesheet,
            {
                {0.1, Rectangle{.x = 0, .y = 0, .width = 32, .height = 8}},
            }
        );
    LogicUtils::gun_idle_idx = LogicUtils::gun_controller->register_animation(gun_idle);

    Utils::Animation* gun_shot = new Utils::Animation(
            &LogicUtils::player_spritesheet,
            {
                {0.05, Rectangle{.x = 0, .y = 0, .width = 32, .height = 8}},
                {0.05, Rectangle{.x = 0, .y = 8, .width = 32, .height = 8}},
                {0.2, Rectangle{.x = 0, .y = 16, .width = 32, .height = 8}},
                {0.05, Rectangle{.x = 0, .y = 32, .width = 32, .height = 8}},
                {0.2, Rectangle{.x = 0, .y = 40, .width = 32, .height = 8}},
                {0.1, Rectangle{.x = 0, .y = 48, .width = 32, .height = 8}},
            }
        );
    LogicUtils::gun_shot_idx = LogicUtils::gun_controller->register_animation(gun_shot);

}

void Pages::GameWindowScene::_loading_update()
{
    BeginDrawing();{
        ClearBackground(RAYWHITE);
        DragonLib::Utils::Drawing::draw_text({
            .content = "Loading",
            .font_size = Global::rem
        });
    }
    EndDrawing();
    return;
}

void Pages::GameWindowScene::_load()
{
    SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

    LogicUtils::player_data.position = {512-LogicUtils::hull_data.player_rectangle.width/2, 288-LogicUtils::hull_data.player_rectangle.height/2};
    LogicUtils::player_data.angle = 0;
    LogicUtils::player_data.health = 5;

    LogicUtils::hull_data.player_speed = 120.0f;
    LogicUtils::hull_data.player_rot_speed = PI/2;
    LogicUtils::hull_data.player_colliding = false;

    LogicUtils::gun_data.gun_angle = 0;
    LogicUtils::gun_data.gun_rot_speed = 1;
    LogicUtils::gun_data.gun_dmg = 1;
    LogicUtils::gun_data.has_shot = false;
    LogicUtils::gun_data.bullet_range = 1000;

    LogicUtils::crosshair_data.tracker_position = Vector2();
    LogicUtils::crosshair_data.tracker_radial_speed = 500;
    LogicUtils::crosshair_data.tracker_radius = 10;
    LogicUtils::crosshair_data.tracker_distance = 0;

    LogicUtils::viewport_data.offset = Vector2();

    LogicUtils::player_spritesheet_image = LoadImage("resources/game_window/tank2_spritesheet.png");
    LogicUtils::player_controller = new Utils::AnimationController();
    LogicUtils::gun_controller = new Utils::AnimationController();

    Global::ServiceProviders::room_client_worker.await();
}

void Pages::GameWindowScene::_cleanup()
{
    UnloadImage(LogicUtils::player_spritesheet_image);
    delete LogicUtils::player_controller;
    delete LogicUtils::gun_controller;
}

void Pages::GameWindowScene::_cleanup_with_context()
{
    UnloadTexture(LogicUtils::player_spritesheet);
}

void Pages::GameWindowScene::logic_update()
{
    pixels_per_unit_x = GetScreenWidth()/1024;
    pixels_per_unit_y = GetScreenHeight()/576;
    LogicUtils::crosshair_data.mouse_position = {GetMousePosition().x/(float)pixels_per_unit_x, GetMousePosition().y/(float)pixels_per_unit_x};
    // set_position(); // For drawing
    LogicUtils::gun_data.has_shot = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    float delta_time = GetFrameTime();

    // Handle movement of gun
    LogicUtils::set_gun_angle(delta_time);

    // Handle movement of tracker circle
    LogicUtils::set_tracker(delta_time);

    // Handle movement
    LogicUtils::handle_movement(delta_time);  


    // Handle gun animation
    if(LogicUtils::gun_data.has_shot)
    {
        LogicUtils::gun_controller->play(LogicUtils::gun_shot_idx, true);
    }
    else if(LogicUtils::gun_controller->current_iteration_count>0)
    {
        LogicUtils::gun_controller->play(LogicUtils::gun_idle_idx, true);
    }
}



void Pages::GameWindowScene::draw_game()
{
    float delta_time = GetFrameTime();
    LogicUtils::player_controller->update(delta_time);
    LogicUtils::gun_controller->update(delta_time);

        // Handle gun animation
    if(LogicUtils::gun_data.has_shot)
    {
        LogicUtils::gun_controller->play(LogicUtils::gun_shot_idx, true);
    }
    else if(LogicUtils::gun_controller->current_iteration_count>0)
    {
        LogicUtils::gun_controller->play(LogicUtils::gun_idle_idx, true);
    }

   // Draw trace
    if (LogicUtils::gun_data.has_shot){
        DrawLineEx(
            {
                512,
                288  
            }, {
                (512 + (float)((LogicUtils::gun_data.bullet_range) * cos(LogicUtils::gun_data.gun_angle)))*(float)pixels_per_unit_x,
                (288 + (float)((LogicUtils::gun_data.bullet_range) * sin(LogicUtils::gun_data.gun_angle)))*(float)pixels_per_unit_y    
            }, 4, RAYWHITE
        );
    }

    // Draw player
    // Color tank_color = player_data.player_colliding ? RED:WHITE;
    Texture* player_texture = LogicUtils::player_controller->get_sprite().first;
    Rectangle* player_source = LogicUtils::player_controller->get_sprite().second;
    DrawTexturePro(*player_texture,
    *player_source,
    {(512)*(float)pixels_per_unit_x, (288)*(float)pixels_per_unit_y, LogicUtils::hull_data.player_rectangle.width, LogicUtils::hull_data.player_rectangle.height},
    {LogicUtils::hull_data.player_rectangle.width/2, LogicUtils::hull_data.player_rectangle.height/2},
    
    (LogicUtils::player_data.angle)*RAD2DEG, WHITE
    );
    
    // Draw gun
    Texture* gun_texture = LogicUtils::gun_controller->get_sprite().first;
    Rectangle* gun_source = LogicUtils::gun_controller->get_sprite().second;
    DrawTexturePro(*gun_texture,
        *gun_source,
        {   (512)*(float)pixels_per_unit_x, (288)*(float)pixels_per_unit_y,
            (LogicUtils::gun_data.gun_rectangle.width)*(float)pixels_per_unit_x, (LogicUtils::gun_data.gun_rectangle.height)*(float)pixels_per_unit_y},
        {
            0,
            (LogicUtils::gun_data.gun_rectangle.height/2)*(float)pixels_per_unit_y
        }, 
        (LogicUtils::gun_data.gun_angle)*RAD2DEG, WHITE
    );

    // Draw gun crosshair circle
    DrawCircleLines((LogicUtils::crosshair_data.tracker_position.x)*(float)pixels_per_unit_x+512, 
        (LogicUtils::crosshair_data.tracker_position.y)*(float)pixels_per_unit_x+288,
        LogicUtils::crosshair_data.tracker_radius,
        LogicUtils::crosshair_data.circle_color
        );
}

void Pages::GameWindowScene::draw_hud()
{
}