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

    // TODO: Send a start ready message
    // Recieve data like spawn point, map etc.

    size_t map_idx = 0;

    Vector2 player_spawn_position = {
        Maps::maps[0].vwidth()/2-hull_data.player_rectangle.width/2,
        Maps::maps[0].vheight()/2-hull_data.player_rectangle.height/2
    };
    camera.init({Maps::maps[0].width(), Maps::maps[0].height()}, {Maps::maps[0].vwidth(), Maps::maps[0].vheight()});
    projected_data.init(player_spawn_position);
    player_data.init(player_spawn_position);
    camera.follow(player_data.position);

    crosshair_data.init();
    hull_data.init();
    gun_data.init();

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
    pixels_per_unit_x = (float)GetScreenWidth()/(Maps::maps[0].vwidth());
    pixels_per_unit_y = (float)GetScreenHeight()/(Maps::maps[0].vheight());
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

    camera.follow(player_data.position);

    bullet_colliding = false;
    // contact_point = {(float)gun_data.bullet_range*(float)cos(gun_data.gun_angle), (float)gun_data.bullet_range*(float)sin(gun_data.gun_angle)};
    // Vector2 curr_contact_point = contact_point;
    for(size_t wall_y = 0; wall_y<Maps::maps[0].map_height_tiles; wall_y++)
    {
        for(size_t wall_x = 0; wall_x<Maps::maps[0].map_width_tiles; wall_x++)
        {
            size_t wall_idx = ((Maps::maps[0].map_width_tiles)*wall_y) + wall_x;
            if (wall_idx >= Maps::maps[0].walls.size()){
                continue;
            }
            if(Maps::maps[0].walls[wall_idx]==0)
            {
                Rectangle wall = {
                    .x = (float)(wall_x)*(Maps::maps[0].tile_width_units),
                    .y = (float)(wall_y)*(Maps::maps[0].tile_width_units),
                    .width = (Maps::maps[0].tile_width_units),
                    .height = (Maps::maps[0].tile_width_units),
                };
                bullet_colliding = Physics::RayVsRect2D(player_data.position, {(float)cos(gun_data.gun_angle), (float)sin(gun_data.gun_angle)}, wall, &contact_point);
                if(bullet_colliding) break;
                // if(bullet_colliding){
                //     if(Vector2Distance(curr_contact_point, player_data.position)<Vector2Distance(contact_point, player_data.position))
                //     {
                //         contact_point = curr_contact_point;
                //         bullet_colliding = false;
                //     }
                // }
            }
            if(bullet_colliding) break;
        }
        if(bullet_colliding) break;
    }
    // Player
    // for(int i=0; i<8; i++)
    // {
    //     if(i!=player_packet.ID)
    //     {
    //         Rectangle other_player_collider = {
    //             .x = old_state[i].position_absolute.x,
    //             .y = old_state[i].position_absolute.y,
    //             .width = hull_data.player_rectangle.width,
    //             .height = hull_data.player_rectangle.height,
    //         };
    //         player_colliding = Physics::sat_collision_detection(collider, projected_data.angle, other_player_collider, old_state[i].player_angle);
    //         if(player_colliding){break;}
    //     }
    //     if(player_colliding){break;}
    // }
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
    DrawTexturePro(
        map,
        {viewport_data.offset.x, viewport_data.offset.y, Maps::maps[0].tiles_in_screen_x*(float)Maps::maps[0].tile_width_units, Maps::maps[0].tiles_in_screen_y*(float)Maps::maps[0].tile_width_units},
        {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        {0,0},
        0,
        WHITE
    );


    // Draw player
    Color tank_color = player_colliding ? RED:WHITE;
    Color gun_color = bullet_colliding ? SKYBLUE:WHITE;
    
    for (auto& pp: LogicUtils::old_state){
        if (!pp.is_alive) continue;
        Texture* player_texture = player_controller->get_sprite().first;
        Rectangle* player_source = player_controller->get_sprite().second;

        hull_data.player_rectangle.x = pp.position_absolute.x;
        hull_data.player_rectangle.y = pp.position_absolute.y;
        std::cout << hull_data.player_rectangle.x << " " << hull_data.player_rectangle.y << std::endl;
        std::cout << "!!!" << player_data.position.x << " " << player_data.position.y << std::endl;
        DrawTexturePro(
            *player_texture,
            *player_source,
            camera.transform(hull_data.player_rectangle),
            camera.scale(Vector2Scale({hull_data.player_rectangle.width, hull_data.player_rectangle.height}, 0.5f)),    
            (pp.player_angle)*RAD2DEG, tank_color
        );
    }
    for (auto& pp: LogicUtils::old_state){
        if (!pp.is_alive) continue;

        // Draw gun
        Texture* gun_texture = gun_controller->get_sprite().first;
        Rectangle* gun_source = gun_controller->get_sprite().second;
        DrawTexturePro(*gun_texture,
            *gun_source,
            camera.transform(Rectangle{
                pp.position_absolute.x, pp.position_absolute.y,
                gun_data.gun_rectangle.width, gun_data.gun_rectangle.height
            }), camera.scale(Vector2{
                0, gun_data.gun_rectangle.height/2
            }), 
            (pp.gun_angle)*RAD2DEG, gun_color
        );
    }
    
    

    // Draw gun crosshair circle
    DrawCircleLinesV(
        Vector2Add(
            camera.scale(crosshair_data.tracker_position),
            {Maps::maps[0].vwidth() / 2, Maps::maps[0].vheight() / 2}
        ),
        crosshair_data.tracker_radius,
        crosshair_data.circle_color
    );
    
   // Draw trace
    if (true){
        DrawLineEx(
            {
                camera.transform(player_data.position),  
            }, {
                (float)((Maps::maps[0].tiles_in_screen_x*Maps::maps[0].tile_width_units)/2 + (float)((gun_data.bullet_range) * cos(gun_data.gun_angle)))*(float)pixels_per_unit_x,
                (float)((Maps::maps[0].tiles_in_screen_y*Maps::maps[0].tile_width_units)/2 + (float)((gun_data.bullet_range) * sin(gun_data.gun_angle)))*(float)pixels_per_unit_y    
            }, 4, RAYWHITE
        );
    }
}

void Pages::GameWindowScene::draw_hud()
{
}