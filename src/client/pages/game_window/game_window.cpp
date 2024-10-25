#include "game_window.hpp"
#define GUN_COOLDOWN_MAX 2
bool player_colliding;
void Pages::GameWindowScene::_update()
{
    
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    // Logic update
    
    logic_update();

    std::cout<<"Health "<<LogicUtils::old_state[LogicUtils::player_packet.ID].last_shot << " "<<LogicUtils::old_timestamps[LogicUtils::player_packet.ID] <<std::endl;

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
    
    for (size_t i = 0; i < Networking::Message::Room::MAX_ROOM_SIZE; ++i){
        
        player_controllers.emplace_back(Utils::AnimationController{});
        gun_controllers.emplace_back(Utils::AnimationController{});
        player_idle_idx = player_controllers[i].register_animation(player_idle);
        player_moving_idx = player_controllers[i].register_animation(player_moving);
        gun_idle_idx = gun_controllers[i].register_animation(gun_idle);
        gun_shot_idx = gun_controllers[i].register_animation(gun_shot);
    }
    
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
    
    for (size_t i = 0; i < Networking::Message::Room::MAX_ROOM_SIZE; ++i){
        old_timestamps.emplace_back(0);
        did_shoots.emplace_back(false);
        contact_pointsss.emplace_back(Vector2{0, 0});
    }
    Global::ServiceProviders::room_client.game_start_callback = [](){
        
        return;
    };
    Global::ServiceProviders::room_client.disconnect_callback = [](){
        SceneManagement::SceneManager::load_deferred(SceneManagement::SceneName::MAIN_PAGE);
        return;
    };
    Global::ServiceProviders::room_client.set_map_callback = [](int map){
        
        return;
    };
    Global::ServiceProviders::room_client.room_broadcast_callback = [](std::vector<std::string> room_members_){
        
        return;
    };
    Global::ServiceProviders::room_client.game_update_callback = [this](void* data){
        update_state((PlayerPacket*)data);
        return;
    };

    leaderboard.push_back({"Player1", 150});
    leaderboard.push_back({"Player2", 120});
    leaderboard.push_back({"Player3", 100});

    std::mutex spawn_mutex;
    std::unique_lock<std::mutex> lock(spawn_mutex);
    std::condition_variable spawn_cv;

    bool received_spawn_data = false;
    Networking::Message::Room::SpawnData data;
    Global::ServiceProviders::room_client.request_spawn([&spawn_cv, &received_spawn_data, &data](Networking::Message::Room::SpawnData* sdata){
        received_spawn_data = true;
        if (sdata != nullptr)
        data = *sdata;
        spawn_cv.notify_all();
    });

    spawn_cv.wait(lock, [&received_spawn_data](){
        return (received_spawn_data || !Global::ServiceProviders::room_client.connected());
    });

    map_idx = data.map_id;
    Vector2 player_spawn_position = data.spawn;

    std::cout << "GOT SPAWN DATA!" <<std::endl;

    hull_data.init();
    camera.init({Maps::maps[map_idx].width(), Maps::maps[map_idx].height()}, {Maps::maps[map_idx].vwidth(), Maps::maps[map_idx].vheight()}, {hull_data.player_rectangle.width, hull_data.player_rectangle.height});
        
    projected_data.init(player_spawn_position);
    player_data.init(player_spawn_position);
    player_data.angle = data.angle;

    camera.follow(player_data.position);
    

    crosshair_data.init();
    gun_data.init();
    

    viewport_data.offset = Vector2();
    

    player_spritesheet_image = LoadImage("resources/game_window/tank2_spritesheet.png");
    map_image = LoadImage("resources/game_window/defaultmap.png");

    time_since_last_send = 0;
    
    
    init_state(Networking::Message::Room::MAX_ROOM_SIZE);
    
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
    LogicUtils::did_shoots.clear();
    LogicUtils::old_timestamps.clear();
    LogicUtils::contact_pointsss.clear();
    UnloadImage(LogicUtils::player_spritesheet_image);
    Global::ServiceProviders::room_client.stop();
    Global::ServiceProviders::room_client_worker.cancel();
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
    // while(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){}
    
    float delta_time = GetFrameTime();
    

    // Handle movement of gun
    
    set_gun_angle(delta_time);
    

    
    // Handle movement of tracker circle
    set_tracker(delta_time);
    

    
    // Handle movement
    handle_movement(delta_time); 
    for (size_t i = 0; i < old_state.size(); ++i){
        if (i == self) continue;
        if (!old_state[i].is_alive) continue;
        player_controllers[i].play(old_state[i].is_idle ? player_idle_idx : player_moving_idx, false);
    }
    
    player_colliding = handle_tank_collision();
    

    // Handle gun animation
    for (size_t i = 0; i < old_state.size(); ++i){
        if (i == self || !old_state[i].is_alive) continue;
        player_controllers[i].play(old_state[i].is_idle ? player_idle_idx : player_moving_idx, false);
    }
    float time = GetTime();
    gun_data.has_shot = gun_data.has_shot && time - LogicUtils::old_timestamps[self] > GUN_COOLDOWN_MAX;
    if(gun_data.has_shot)
    {
        player_packet.last_shot = time;
        gun_controllers[self].play(gun_shot_idx, true);
     
    }
    else if(gun_controllers[self].current_iteration_count>0)
    {
        gun_controllers[self].play(gun_idle_idx, true);

    }

    
    for (size_t i = 0; i < old_state.size(); ++i){
        if (i == self || !old_state[i].is_alive) continue;
        
        if (did_shoots[i]){
            gun_controllers[i].play(gun_shot_idx, true);
        } else if(gun_controllers[i].current_iteration_count>0){
            gun_controllers[i].play(gun_idle_idx, true);
        }
    }
    
    
    time_since_last_send += delta_time;
    
    if(time_since_last_send>=0.017)
    {   
        set_packet();
        time_since_last_send = 0;        
        Global::ServiceProviders::room_client.request_game_update(&player_packet, sizeof(PlayerPacket));
    }

    
    camera.follow(player_data.position);

    bullet_colliding = false;
    contact_point = {player_data.position.x + (float)gun_data.bullet_range*(float)cos(gun_data.gun_angle), player_data.position.y + (float)gun_data.bullet_range*(float)sin(gun_data.gun_angle)};
    Vector2 curr_contact_point = contact_point;
    if (gun_data.has_shot){
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
                        .x = ((float)wall_x - 1.0f)*(Maps::maps[0].tile_width_units),
                        .y = ((float)wall_y - 0.75f)*(Maps::maps[0].tile_width_units),
                        .width = (Maps::maps[0].tile_width_units),
                        .height = (Maps::maps[0].tile_width_units),
                    };
                    bullet_colliding = Physics::CheckCollisionRay2dRect(player_data.position, gun_data.gun_angle, wall, &curr_contact_point);
                    
                    if(bullet_colliding){
                        
                        if(Vector2Distance(curr_contact_point, player_data.position)<Vector2Distance(contact_point, player_data.position))
                        {
                            contact_point = curr_contact_point;
                        }
                    }
                }
            }
        }
        player_packet.closest_wall_hit = contact_point;
    }
    // Player
    if (gun_data.has_shot){
        // playe rhas shot
        for(int i=0; i<12; i++)
        {
            if(i!=player_packet.ID && old_state[i].is_alive)
            {
                Rectangle other_player_collider = {
                    .x = old_state[i].position_absolute.x - 1.0f*(Maps::maps[0].tile_width_units),
                    .y = old_state[i].position_absolute.y - 0.75f * (Maps::maps[0].tile_width_units),
                    .width = hull_data.player_rectangle.width,
                    .height = hull_data.player_rectangle.height,
                };

                
                bullet_colliding = Physics::CheckCollisionRay2dRectEx(player_data.position, gun_data.gun_angle, other_player_collider, old_state[i].player_angle, &curr_contact_point);
                // bullet_colliding = Physics::CheckCollisionRay2dRect(player_data.position, gun_data.gun_angle, other_player_collider, &curr_contact_point);
            
                if(bullet_colliding){
                    
                    if(Vector2Distance(curr_contact_point, player_data.position)<Vector2Distance(contact_point, player_data.position))
                    {
                        contact_point = curr_contact_point;
                    }
                }
            }
        }
    }
    
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
        camera.viewport(),
        {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        {0,0},
        0,
        WHITE
    );

    for (size_t i = 0; i < old_state.size(); ++i){
        if(did_shoots[i])
        {
            DrawLineEx(
                camera.transform(old_state[i].position_absolute),
                camera.transform(contact_pointsss[i]),
                4, WHITE
            );
        }
    }
    

    // if (player_packet.last_shot - LogicUtils::old_timestamps[self] > GUN_COOLDOWN_MAX){
    //     DrawLineEx(
    //         camera.transform(player_data.position),
    //         camera.transform(contact_point),
    //         4, WHITE
    //     );
    // }

    // Draw player
    Color tank_color = player_colliding ? RED:WHITE;
    Color gun_color = bullet_colliding ? SKYBLUE:WHITE;
    
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
        if (!pp.is_alive) continue;
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
            (pp.gun_angle)*RAD2DEG, gun_color
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
            (gun_data.gun_angle)*RAD2DEG, gun_color
        );
    }

    
    // Draw gun crosshair circle
    DrawCircleLinesV(
        camera.transform(
            Vector2Add(crosshair_data.tracker_position, player_data.position)
        ),
        crosshair_data.tracker_radius,
        crosshair_data.circle_color
    );
}

void Pages::GameWindowScene::draw_hud()
{
    
}