#include "game_window.hpp"
#include <algorithm>
#define GUN_COOLDOWN_MAX 0.5
float time1;
bool player_colliding;
void Pages::GameWindowScene::_update()
{
    
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    // Logic update
    

    if (LogicUtils::old_state[LogicUtils::player_packet.ID].is_alive){
        logic_update();
    }
    else {
        dead_timer += GetFrameTime();
    }

    if (dead_timer > 5 && !spawn_request_setter_worker.is_running()){
        using namespace LogicUtils;
        
        spawn_request_setter_worker.accomplish([this](const bool& cancelled){
            std::mutex spawn_mutex;
            std::unique_lock<std::mutex> lock(spawn_mutex);
            std::condition_variable spawn_cv;
            Networking::Message::Room::SpawnData data;
            bool received_spawn_data = false;
            Global::ServiceProviders::room_client.request_spawn([&spawn_cv, &received_spawn_data, &data](Networking::Message::Room::SpawnData* sdata){
                received_spawn_data = true;
                if (sdata != nullptr)
                data = *sdata;
                spawn_cv.notify_all();
            });

            spawn_cv.wait(lock, [&cancelled, &received_spawn_data](){
                return (received_spawn_data || cancelled || !Global::ServiceProviders::room_client.connected());
            });

            hull_data.init();
            camera.init({Maps::maps[map_idx].width(), Maps::maps[map_idx].height()}, {Maps::maps[map_idx].vwidth(), Maps::maps[map_idx].vheight()}, {hull_data.player_rectangle.width, hull_data.player_rectangle.height});
            if (cancelled || !Global::ServiceProviders::room_client.connected()){
                map_idx = 0;
            } else{

                map_idx = data.map_id;
                Vector2 player_spawn_position = data.spawn;
                projected_data.init(player_spawn_position);
                player_data.init(player_spawn_position);
                player_data.angle = data.angle;
            }

            std::cout << "GOT SPAWN DATA!" <<std::endl;

            LogicUtils::old_state[LogicUtils::player_packet.ID].is_alive = true;

            camera.follow(player_data.position);
            

            crosshair_data.init();
            gun_data.init();
            

            viewport_data.offset = Vector2();
            dead_timer = 0;
        });
        
    }



    time_since_last_send += GetFrameTime();
    
    if(time_since_last_send>=0.017)
    {   
        LogicUtils::set_packet();
        time_since_last_send = 0;        
        Global::ServiceProviders::room_client.request_game_update(&LogicUtils::player_packet, sizeof(LogicUtils::PlayerPacket));
    }
    


    // Drawing
    BeginDrawing();{
        ClearBackground(SKYBLUE);
        draw_game();
        
        draw_hud();
        
        if (dead_timer > 0){
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0xcb, 0xc6, 0xb2, 0x90});
            DragonLib::Utils::Drawing::draw_textbox({
                .content = std::string("Respawning in....") + std::to_string((int)(6 - dead_timer)),
                .font_size = Global::rem * 1.5f,
                .font_color = BLACK,
            });
        }
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
    spawn_request_setter_worker.await();
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
        Global::names = room_members_;
        return;
    };
    Global::ServiceProviders::room_client.game_update_callback = [this](void* data){
        
        update_state((PlayerPacket*)data);
        return;
    };


    player_spritesheet_image = LoadImage("resources/game_window/tank2_spritesheet.png");
    map_image = LoadImage("resources/game_window/defaultmap.png");

    spawn_request_setter_worker.accomplish([this](const bool& cancelled){
        std::mutex spawn_mutex;
        std::unique_lock<std::mutex> lock(spawn_mutex);
        std::condition_variable spawn_cv;
        Networking::Message::Room::SpawnData data;
        bool received_spawn_data = false;
        Global::ServiceProviders::room_client.request_spawn([&spawn_cv, &received_spawn_data, &data](Networking::Message::Room::SpawnData* sdata){
            received_spawn_data = true;
            if (sdata != nullptr)
            data = *sdata;
            spawn_cv.notify_all();
        });

        spawn_cv.wait(lock, [&cancelled, &received_spawn_data](){
            return (received_spawn_data || cancelled || !Global::ServiceProviders::room_client.connected());
        });

        hull_data.init();
        camera.init({Maps::maps[map_idx].width(), Maps::maps[map_idx].height()}, {Maps::maps[map_idx].vwidth(), Maps::maps[map_idx].vheight()}, {hull_data.player_rectangle.width, hull_data.player_rectangle.height});
        if (cancelled || !Global::ServiceProviders::room_client.connected()){
            map_idx = 0;
        } else{

            map_idx = data.map_id;
            Vector2 player_spawn_position = data.spawn;
            projected_data.init(player_spawn_position);
            player_data.init(player_spawn_position);
            player_data.angle = data.angle;
        }

        std::cout << "GOT SPAWN DATA!" <<std::endl;

            

        camera.follow(player_data.position);
        

        crosshair_data.init();
        gun_data.init();
        

        viewport_data.offset = Vector2();
    });
    spawn_request_setter_worker.await();

    std::cout << "HELLO" << std::endl;
    
    


    time_since_last_send = 0;
    
    
    init_state(Networking::Message::Room::MAX_ROOM_SIZE);
    LogicUtils::old_state[LogicUtils::player_packet.ID].is_alive = true;
    
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
    // for (size_t i = 0; i < leaderboard.size(); i++) {
    //     std::string playerText = leaderboard[i].name + ": " + std::to_string(leaderboard[i].score);
    //     DrawText(playerText.c_str(), xPos + 10, textY, 18, RAYWHITE);
    //     textY += 30;  // Move down for the next player
    // }

    std::vector <std::pair<std::string,int>> arr;

    for(int i = 0; i<12; i ++){
        if(Global::names[i].empty())continue;
            arr.push_back(std::make_pair(Global::names[i],LogicUtils::old_state[i].score));
    } 

    std::sort(arr.begin(), arr.end(), [](const auto& a, const auto& b) { return a.second > b.second;});

    for(auto it: arr){
        std::string playerText = it.first + " " + std::to_string(it.second);
        DrawText(playerText.c_str(), xPos + 10, textY, 18, RAYWHITE);
        textY += 30;  // Move down for the next player
    
    }
}


void Pages::GameWindowScene::_cleanup()
{
    spawn_request_setter_worker.cancel();
    spawn_request_setter_worker.await();
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

    if (gun_data.has_shot){
        DrawLineEx(
            camera.transform(player_data.position),
            camera.transform(contact_point),
            4, WHITE
        );
    }
    for (size_t i = 0; i < old_state.size(); ++i){
        if (i == self) continue;
        if(did_shoots[i])
        {
            old_timestamps[i] = old_state[i].last_shot;
            DrawLineEx(
                camera.transform(old_state[i].position_absolute),
                camera.transform(contact_pointsss[i]),
                4, WHITE
            );
        }
    }
    

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

        std::cout << LogicUtils::old_state[i].health << std::endl;
        draw_name_health(i);
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
        draw_name_health(self);
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

void Pages::GameWindowScene::draw_name_health(int i){
    using namespace LogicUtils;

    float factor = (float)old_state[i].health/(float)max_health;



    health_bar = {
        .x = i == player_packet.ID ? player_data.position.x : old_state[i].position_absolute.x,
        .y = i == player_packet.ID ? player_data.position.y : old_state[i].position_absolute.y,
        .width = 100,
        .height = 10
    };

    

    health_bar = camera.transform(health_bar);
    Vector2 offset = camera.scale(Vector2{-50, -hull_data.player_rectangle.height});
    
    health_bar.x += offset.x;
    health_bar.y += offset.y;
    DrawRectangleRounded(health_bar, 1.0, 6, RED);

    float actual_width = health_bar.width;

    health_bar.width *= factor; 
    DrawRectangleRounded(health_bar, 5, 6, GREEN);
    
    auto rec = DragonLib::Utils::Drawing::draw_text({
        .content = Global::names[i],
        .sdf=false,
        .font_size = Global::rem * 0.5f,
        .font_color = BLACK,
        .position = {.value = {health_bar.x + actual_width/2, health_bar.y + health_bar.height/2}, .mode = {DragonLib::UI::DrawParameters::ABSOLUTE, DragonLib::UI::DrawParameters::ABSOLUTE}},
    });
}

void Pages::GameWindowScene::draw_hud()
{
    
}