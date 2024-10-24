#include "game_window.hpp"
bool player_colliding;
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
    using namespace LogicUtils;
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

    map = LoadTextureFromImage(map_image);
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
    using namespace LogicUtils;
    SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

    Global::ServiceProviders::room_client.game_start_callback = [](){
        std::cout << "SOMETHING BAD HAPPEND SIR" << std::endl;
        return;
    };
    Global::ServiceProviders::room_client.disconnect_callback = [](){
        SceneManagement::SceneManager::load_deferred(SceneManagement::SceneName::MAIN_PAGE);
        return;
    };
    Global::ServiceProviders::room_client.set_map_callback = [](int map){
        std::cout << "SOMETHING BAD HAPPEND SIR V2" << std::endl;
        return;
    };
    Global::ServiceProviders::room_client.room_broadcast_callback = [](std::vector<std::string> room_members_){
        std::cout << "SOMETHING BAD HAPPEND SIR V3" << std::endl;
        return;
    };
    Global::ServiceProviders::room_client.game_update_callback = [this](void* data){
        PlayerPacket* game_state = (PlayerPacket*)data;
        update_state(game_state);
        return;
    };

    player_data.position = {(float)(Maps::map1.tiles_in_screen_x*Maps::map1.tile_width_units)/2-hull_data.player_rectangle.width/2,
    (float)(Maps::map1.tiles_in_screen_y*Maps::map1.tile_width_units)/2-hull_data.player_rectangle.height/2};
    player_data.angle = 0;
    player_data.health = 5;
    projected_data.position = {(float)(Maps::map1.tiles_in_screen_x*Maps::map1.tile_width_units)-hull_data.player_rectangle.width/2,
    (float)(Maps::map1.tiles_in_screen_y*Maps::map1.tile_width_units)-hull_data.player_rectangle.height/2};
    projected_data.angle = 0;
    projected_data.health = 5;

    hull_data.player_speed = 120.0f;
    hull_data.player_rot_speed = PI/2;
    hull_data.player_colliding = false;

    gun_data.gun_angle = 0;
    gun_data.gun_rot_speed = 1;
    gun_data.gun_dmg = 1;
    gun_data.has_shot = false;
    gun_data.bullet_range = 1000;

    crosshair_data.tracker_position = Vector2();
    crosshair_data.tracker_radial_speed = 500;
    crosshair_data.tracker_radius = 10;
    crosshair_data.tracker_distance = 0;

    viewport_data.offset = Vector2();

    player_spritesheet_image = LoadImage("resources/game_window/tank2_spritesheet.png");
    map_image = LoadImage("resources/game_window/defaultmap.png");
    player_controller = new Utils::AnimationController();
    gun_controller = new Utils::AnimationController();

    time_since_last_send = 0;
    init_state(Networking::Message::Room::MAX_ROOM_SIZE);
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
    using namespace LogicUtils;
    pixels_per_unit_x = (float)GetScreenWidth()/(Maps::map1.tiles_in_screen_x*(float)Maps::map1.tile_width_units);
    pixels_per_unit_y = (float)GetScreenHeight()/(Maps::map1.tiles_in_screen_y*(float)Maps::map1.tile_width_units);
    crosshair_data.mouse_position = {GetMousePosition().x/(float)pixels_per_unit_x, GetMousePosition().y/(float)pixels_per_unit_y};
    // set_position(); // For drawing
    gun_data.has_shot = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    float delta_time = GetFrameTime();

    // Handle movement of gun
    set_gun_angle(delta_time);

    // Handle movement of tracker circle
    set_tracker(delta_time);

    // Handle movement
    handle_movement(delta_time);  

    player_colliding = handle_tank_collision();

    // Handle gun animation
    if(gun_data.has_shot)
    {
        gun_controller->play(gun_shot_idx, true);
    }
    else if(gun_controller->current_iteration_count>0)
    {
        gun_controller->play(gun_idle_idx, true);
    }

    set_packet();
    time_since_last_send += delta_time;
    if(time_since_last_send>0.017)
    {
        time_since_last_send = 0;
        Global::ServiceProviders::room_client.request_game_update(&player_packet, sizeof(PlayerPacket));
    }
}



void Pages::GameWindowScene::draw_game()
{
    using namespace LogicUtils;
    float delta_time = GetFrameTime();
    player_controller->update(delta_time);
    gun_controller->update(delta_time);

    // Handle gun animation
    if(gun_data.has_shot)
    {
        gun_controller->play(gun_shot_idx, true);
    }
    else if(gun_controller->current_iteration_count>0)
    {
        gun_controller->play(gun_idle_idx, true);
    }

    // Draw bg
    DrawTexturePro(map,
        {viewport_data.offset.x, viewport_data.offset.y, Maps::map1.tiles_in_screen_x*(float)Maps::map1.tile_width_units, Maps::map1.tiles_in_screen_y*(float)Maps::map1.tile_width_units},
        {0, 0, Maps::map1.tiles_in_screen_x*(float)Maps::map1.tile_width_units*(float)pixels_per_unit_x, Maps::map1.tiles_in_screen_y*(float)Maps::map1.tile_width_units*(float)pixels_per_unit_y},
        {0,0},
        0,
        WHITE
    );


    // Draw player
    Color tank_color = player_colliding ? RED:WHITE;
    Texture* player_texture = player_controller->get_sprite().first;
    Rectangle* player_source = player_controller->get_sprite().second;
    DrawTexturePro(*player_texture,
    *player_source,
    {(float)((Maps::map1.tiles_in_screen_x*Maps::map1.tile_width_units)/2)*(float)pixels_per_unit_x, (float)((Maps::map1.tiles_in_screen_y*Maps::map1.tile_width_units)/2)*(float)pixels_per_unit_y, hull_data.player_rectangle.width*(float)pixels_per_unit_x, hull_data.player_rectangle.height*(float)pixels_per_unit_y},
    {hull_data.player_rectangle.width/2*(float)pixels_per_unit_x, hull_data.player_rectangle.height/2*(float)pixels_per_unit_y},
    
    (player_data.angle)*RAD2DEG, tank_color
    );
    
    // Draw gun
    Texture* gun_texture = gun_controller->get_sprite().first;
    Rectangle* gun_source = gun_controller->get_sprite().second;
    DrawTexturePro(*gun_texture,
        *gun_source,
        {   (float)((Maps::map1.tiles_in_screen_x*Maps::map1.tile_width_units)/2)*(float)pixels_per_unit_x, (float)((Maps::map1.tiles_in_screen_y*Maps::map1.tile_width_units)/2)*(float)pixels_per_unit_y,
            (gun_data.gun_rectangle.width)*(float)pixels_per_unit_x, (gun_data.gun_rectangle.height)*(float)pixels_per_unit_y},
        {
            0,
            (gun_data.gun_rectangle.height/2)*(float)pixels_per_unit_y
        }, 
        (gun_data.gun_angle)*RAD2DEG, WHITE
    );

    // Draw gun crosshair circle
    DrawCircleLines(
        ((crosshair_data.tracker_position.x)+(Maps::map1.tiles_in_screen_x*Maps::map1.tile_width_units)/2)*(float)pixels_per_unit_x, 
        ((crosshair_data.tracker_position.y)+(Maps::map1.tiles_in_screen_y*Maps::map1.tile_width_units)/2)*(float)pixels_per_unit_y,
        crosshair_data.tracker_radius,
        crosshair_data.circle_color
        );
   // Draw trace
    if (true){
        DrawLineEx(
            {
                ((float)(Maps::map1.tiles_in_screen_x*Maps::map1.tile_width_units)/2)*(float)pixels_per_unit_x,
                ((float)(Maps::map1.tiles_in_screen_y*Maps::map1.tile_width_units)/2)*(float)pixels_per_unit_y
  
            }, {
                (float)((Maps::map1.tiles_in_screen_x*Maps::map1.tile_width_units)/2 + (float)((gun_data.bullet_range) * cos(gun_data.gun_angle)))*(float)pixels_per_unit_x,
                (float)((Maps::map1.tiles_in_screen_y*Maps::map1.tile_width_units)/2 + (float)((gun_data.bullet_range) * sin(gun_data.gun_angle)))*(float)pixels_per_unit_y    
            }, 4, RAYWHITE
        );
    }
}

void Pages::GameWindowScene::draw_hud()
{
}