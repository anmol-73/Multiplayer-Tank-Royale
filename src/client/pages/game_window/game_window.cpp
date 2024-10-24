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
        draw_leaderboard();
    }
    EndDrawing();

    return;
}

void Pages::GameWindowScene::_load_with_context()
{
    
    using namespace LogicUtils;
    player_spritesheet = LoadTextureFromImage(player_spritesheet_image);
    
    std::cout<< "WTD" <<std::endl;
    player_idle = new Utils::Animation(
        &player_spritesheet,
        {
            {0.3, Rectangle{.x = 32, .y = 0, .width = 32, .height = 24}},
        }
    );
    player_moving = new Utils::Animation(
        &player_spritesheet,
        {
            {0.3, Rectangle{.x = 32, .y = 0, .width = 32, .height = 24}},
            {0.3, Rectangle{.x = 32, .y = 24, .width = 32, .height = 24}}
        }
    );
    gun_idle = new Utils::Animation(
        &player_spritesheet,
        {
            {0.1, Rectangle{.x = 0, .y = 0, .width = 32, .height = 8}},
        }
    );
    gun_shot = new Utils::Animation(
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
    std::cout<< "WTF" <<std::endl;
    for (size_t i = 0; i < Networking::Message::Room::MAX_ROOM_SIZE; ++i){
        std::cout<< "WTF2" <<std::endl;
        player_controllers.emplace_back(Utils::AnimationController{});
        gun_controllers.emplace_back(Utils::AnimationController{});
        player_idle_idx = player_controllers[i].register_animation(player_idle);
        player_moving_idx = player_controllers[i].register_animation(player_moving);
        gun_idle_idx = gun_controllers[i].register_animation(gun_idle);
        gun_shot_idx = gun_controllers[i].register_animation(gun_shot);

    }
    std::cout<< "WTF" <<std::endl;
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

    leaderboard.push_back({"Player1", 150});
    leaderboard.push_back({"Player2", 120});
    leaderboard.push_back({"Player3", 100});

    // TODO: Send a start ready message
    // Recieve data like spawn point, map etc.

    size_t map_idx = 0;

    Vector2 player_spawn_position = {
        Maps::maps[0].vwidth()/2-hull_data.player_rectangle.width/2,
        Maps::maps[0].vheight()/2-hull_data.player_rectangle.height/2
    };
    std::cout<< "WTD" <<std::endl;
    camera.init({Maps::maps[0].width(), Maps::maps[0].height()}, {Maps::maps[0].vwidth(), Maps::maps[0].vheight()});
    projected_data.init(player_spawn_position);
    player_data.init(player_spawn_position);
    camera.follow(player_data.position);
    std::cout<< "WTD" <<std::endl;

    crosshair_data.init();
    hull_data.init();
    gun_data.init();
    std::cout<< "WTD" <<std::endl;

    viewport_data.offset = Vector2();
    std::cout<< "WTD" <<std::endl;

    player_spritesheet_image = LoadImage("resources/game_window/tank2_spritesheet.png");
    map_image = LoadImage("resources/game_window/defaultmap.png");

    time_since_last_send = 0;
    std::cout<< "WTD" <<std::endl;
    
    init_state(Networking::Message::Room::MAX_ROOM_SIZE);
    std::cout<< "WTD" <<std::endl;
}

void Pages::GameWindowScene::draw_leaderboard() {
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Define the position and size of the leaderboard window
    int leaderboardWidth = 200;
    int leaderboardHeight = 200;
    int padding = 10;
    int xPos = screenWidth - leaderboardWidth - padding; // Top-right corner
    int yPos = padding;

    // Draw a semi-transparent background for the leaderboard
    DrawRectangle(xPos, yPos, leaderboardWidth, leaderboardHeight, Fade(DARKGRAY, 0.7f));

    // Draw the title "Leaderboard"
    DrawText("Leaderboard", xPos + 10, yPos + 10, 20, RAYWHITE);

    // Draw each player's name and score
    int textY = yPos + 40;
    for (size_t i = 0; i < leaderboard.size(); i++) {
        std::string playerText = leaderboard[i].name + ": " + std::to_string(leaderboard[i].score);
        DrawText(playerText.c_str(), xPos + 10, textY, 18, RAYWHITE);
        textY += 30;  // Move down for the next player
    }
}


void Pages::GameWindowScene::_cleanup()
{
    UnloadImage(LogicUtils::player_spritesheet_image);
}

void Pages::GameWindowScene::_cleanup_with_context()
{
    UnloadTexture(LogicUtils::player_spritesheet);
    delete LogicUtils::player_idle;
    delete LogicUtils::player_moving;
    delete LogicUtils::gun_idle;
    delete LogicUtils::gun_shot;
    LogicUtils::gun_controllers.clear();
    LogicUtils::player_controllers.clear();
}

void Pages::GameWindowScene::logic_update()
{
    using namespace LogicUtils;
    size_t self = Global::ServiceProviders::room_client.get_id();
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
        gun_controllers[self].play(gun_shot_idx, true);
    }
    else if(gun_controllers[self].current_iteration_count>0)
    {
        gun_controllers[self].play(gun_idle_idx, true);
    }

    set_packet();
    time_since_last_send += delta_time;
    if(time_since_last_send>0.017)
    {
        time_since_last_send = 0;
        Global::ServiceProviders::room_client.request_game_update(&player_packet, sizeof(PlayerPacket));
    }

    camera.follow(player_data.position);
}



void Pages::GameWindowScene::draw_game()
{
    using namespace LogicUtils;
    float delta_time = GetFrameTime();

    for (auto& controller: player_controllers) controller.update(delta_time);
    for (auto& controller: gun_controllers) controller.update(delta_time);
    size_t self = Global::ServiceProviders::room_client.get_id();

    

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

    
    for (size_t i = 0; i < old_state.size(); ++i){
        if (i == self) continue;
        auto& pp = old_state[i];
        if (!pp.is_alive) continue;
        Texture* player_texture = player_controllers[i].get_sprite().first;
        Rectangle* player_source = player_controllers[i].get_sprite().second;

        hull_data.player_rectangle.x = pp.position_absolute.x;
        hull_data.player_rectangle.y = pp.position_absolute.y;
        DrawTexturePro(
            *player_texture,
            *player_source,
            camera.transform(hull_data.player_rectangle),
            camera.scale(Vector2Scale({hull_data.player_rectangle.width, hull_data.player_rectangle.height}, 0.5f)),    
            (pp.player_angle)*RAD2DEG, PURPLE
        );
    }
    if (old_state[self].is_alive){
        Texture* player_texture = player_controllers[self].get_sprite().first;
        Rectangle* player_source = player_controllers[self].get_sprite().second;

        hull_data.player_rectangle.x = player_data.position.x;
        hull_data.player_rectangle.y = player_data.position.y;
        DrawTexturePro(
            *player_texture,
            *player_source,
            camera.transform(hull_data.player_rectangle),
            camera.scale(Vector2Scale({hull_data.player_rectangle.width, hull_data.player_rectangle.height}, 0.5f)),    
            (player_data.angle)*RAD2DEG, tank_color
        );
    }

    for (size_t i = 0; i < old_state.size(); ++i){
        if (i == self) continue;
        auto& pp = old_state[i];
        // Draw gun
        Texture* gun_texture = gun_controllers[i].get_sprite().first;
        Rectangle* gun_source = gun_controllers[i].get_sprite().second;
        DrawTexturePro(*gun_texture,
            *gun_source,
            camera.transform(Rectangle{
                pp.position_absolute.x, pp.position_absolute.y,
                gun_data.gun_rectangle.width, gun_data.gun_rectangle.height
            }), camera.scale(Vector2{
                0, gun_data.gun_rectangle.height/2
            }), 
            (pp.gun_angle)*RAD2DEG, PURPLE
        );
    }
    
    if (old_state[self].is_alive){
        Texture* gun_texture = gun_controllers[self].get_sprite().first;
        Rectangle* gun_source = gun_controllers[self].get_sprite().second;
        DrawTexturePro(*gun_texture,
            *gun_source,
            camera.transform(Rectangle{
                player_data.position.x, player_data.position.y,
                gun_data.gun_rectangle.width, gun_data.gun_rectangle.height
            }), camera.scale(Vector2{
                0, gun_data.gun_rectangle.height/2
            }), 
            (gun_data.gun_angle)*RAD2DEG, WHITE
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
                ((float)(Maps::maps[0].tiles_in_screen_x*Maps::maps[0].tile_width_units)/2)*(float)pixels_per_unit_x,
                ((float)(Maps::maps[0].tiles_in_screen_y*Maps::maps[0].tile_width_units)/2)*(float)pixels_per_unit_y
  
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