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
    player_spritesheet = LoadTextureFromImage(player_spritesheet_image);

    Utils::Animation* player_idle = new Utils::Animation(
            &player_spritesheet,
            {
                {0.3, Rectangle{.x = 32, .y = 0, .width = 32, .height = 24}},
            }
        );
    player_idle_idx = player_controller->register_animation(player_idle);

    Utils::Animation* player_moving = new Utils::Animation(
            &player_spritesheet,
            {
                {0.3, Rectangle{.x = 32, .y = 0, .width = 32, .height = 24}},
                {0.3, Rectangle{.x = 32, .y = 24, .width = 32, .height = 24}}
            }
        );
    player_moving_idx = player_controller->register_animation(player_moving);
    
    Utils::Animation* gun_idle = new Utils::Animation(
            &player_spritesheet,
            {
                {0.1, Rectangle{.x = 0, .y = 0, .width = 32, .height = 8}},
            }
        );
    gun_idle_idx = gun_controller->register_animation(gun_idle);

    Utils::Animation* gun_shot = new Utils::Animation(
            &player_spritesheet,
            {
                {0.05, Rectangle{.x = 0, .y = 0, .width = 32, .height = 8}},
                {0.05, Rectangle{.x = 0, .y = 8, .width = 32, .height = 8}},
                {0.2, Rectangle{.x = 0, .y = 16, .width = 32, .height = 8}},
                {0.05, Rectangle{.x = 0, .y = 32, .width = 32, .height = 8}},
                {0.2, Rectangle{.x = 0, .y = 40, .width = 32, .height = 8}},
                {0.1, Rectangle{.x = 0, .y = 48, .width = 32, .height = 8}},
            }
        );
    gun_shot_idx = gun_controller->register_animation(gun_shot);

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

    LogicUtils::player_data.position = {512, 288};
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

    LogicUtils::viewport_data.offset = Vector2();

    player_spritesheet_image = LoadImage("resources/game_window/tank2_spritesheet.png");
    player_controller = new Utils::AnimationController();
    gun_controller = new Utils::AnimationController();
}

void Pages::GameWindowScene::_cleanup()
{
    UnloadImage(player_spritesheet_image);
    delete player_controller;
    delete gun_controller;
}

void Pages::GameWindowScene::_cleanup_with_context()
{
    UnloadTexture(player_spritesheet);
}

void Pages::GameWindowScene::logic_update()
{
    LogicUtils::crosshair_data.mouse_position = GetMousePosition();
    // set_position(); // For drawing

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
        gun_controller->play(gun_shot_idx, true);
    }
    else if(gun_controller->current_iteration_count>0)
    {
        gun_controller->play(gun_idle_idx, true);
    }
}



void Pages::GameWindowScene::draw_game()
{
    float delta_time = GetFrameTime();
    player_controller->update(delta_time);
    gun_controller->update(delta_time);

   // Draw trace
    if (LogicUtils::gun_data.has_shot){
        DrawLineEx(
            {
                LogicUtils::player_data.position.x - LogicUtils::viewport_data.offset.x,
                LogicUtils::player_data.position.y + LogicUtils::viewport_data.offset.y   
            }, {
                LogicUtils::player_data.position.x - LogicUtils::viewport_data.offset.x + (float)((LogicUtils::gun_data.bullet_range) * cos(LogicUtils::gun_data.gun_angle)),
                LogicUtils::player_data.position.y + LogicUtils::viewport_data.offset.y  + (float)((LogicUtils::gun_data.bullet_range) * cos(LogicUtils::gun_data.gun_angle))    
            }, 4, RAYWHITE
        );
    }

    // Draw player
    // Color tank_color = player_data.player_colliding ? RED:WHITE;
    Texture* player_texture = player_controller->get_sprite().first;
    Rectangle* player_source = player_controller->get_sprite().second;
    DrawTexturePro(*player_texture,
    *player_source,
    {LogicUtils::player_data.position.x, LogicUtils::player_data.position.y, LogicUtils::hull_data.player_rectangle.width, LogicUtils::hull_data.player_rectangle.height},
    {LogicUtils::hull_data.player_rectangle.width/2, LogicUtils::hull_data.player_rectangle.height/2},
    
    (LogicUtils::player_data.angle)*RAD2DEG, WHITE
    );
    
    // Draw gun
    Texture* gun_texture = gun_controller->get_sprite().first;
    Rectangle* gun_source = gun_controller->get_sprite().second;
    DrawTexturePro(*gun_texture,
        *gun_source,
        LogicUtils::gun_data.gun_rectangle,
        {
            LogicUtils::gun_data.gun_rectangle.width/2,
            LogicUtils::gun_data.gun_rectangle.height
        }, 
        (LogicUtils::gun_data.gun_angle)*RAD2DEG, WHITE
    );

    // Draw gun crosshair circle
    DrawCircleLines(LogicUtils::crosshair_data.tracker_position.x-LogicUtils::viewport_data.offset.x, 
        LogicUtils::crosshair_data.tracker_position.y-LogicUtils::viewport_data.offset.x,
        LogicUtils::crosshair_data.tracker_radius,
        LogicUtils::crosshair_data.circle_color
        );
}

void Pages::GameWindowScene::draw_hud()
{
}