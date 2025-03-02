#include "game_window.hpp"


void Pages::GameWindowScene::_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    logic_update();

    BeginDrawing();{
        ClearBackground({0x1e, 0x1e, 0x1e, 0xff});
    }
    EndDrawing();

}

void Pages::GameWindowScene::_loading_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    BeginDrawing();{
        ClearBackground({0xcb, 0xc6, 0xb2, 0xe0});
        DragonLib::Utils::Drawing::draw_text({
            .content = "Starting Game...   :)",
            .font_size = Global::rem * 2,
            .font_color = {0, 0, 0, 0x60}
        });
    }
    EndDrawing();
}

void Pages::GameWindowScene::_prepare(const void *msg, size_t command)
{
    std::cout << command << std::endl;
    switch(command){
        case 0: {
            assert(msg != nullptr);
            address = *static_cast<const Communication::Address*>(msg);
            std::cout << address.port << ' ' << address.name << std::endl;
            break;
        }
        
        case 1: {
            assert(msg != nullptr);
            player_details_size = *static_cast<const size_t*>(msg);
            std::cout << "SZ " << player_details_size << std::endl;
            break;
        }
        
        case 2: {
            assert(msg != nullptr);
            auto ptr = static_cast<const PlayerDetail*>(msg);
            player_details.assign(ptr, ptr + player_details_size);
            std::cout << "SZ 2 " << player_details.size() << std::endl;
            break;
        }
        
        case 3: {
            assert(msg != nullptr);
            settings = *static_cast<const Communication::Room::RoomSettings*>(msg);
            std::cout << "setging " << settings.map << std::endl;
            break;
        }

        default: assert(false);
    }
}

void Pages::GameWindowScene::_load()
{
    client = new ServiceConsumers::GameClient([this](const Game::GameState server_gs, size_t size)->void{game_update_callback(server_gs, size);});
    std::string error = client->connect(address);

    if (error.size() > 0){
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::SPLASH, error.c_str(), 0);
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::SPLASH, 0);
        return;
    }

    client_worker.accomplish([this](auto _){
        client->start();
    });

    curr_frame.frame_num = 0;

    camera.follow(game_state.player_vector[player_id].position);

    // TODO: gotta sqitch this stuff
    player_spritesheet_image = LoadImage("resources/game_window/tank2_spritesheet.png");
    map_image = LoadImage("resources/game_window/defaultmap.png");


    crosshair_data.init();
}

void Pages::GameWindowScene::_cleanup()
{
    UnloadImage(player_spritesheet_image);
    if (client != nullptr){
        client->stop();
        client_worker.await();
        delete client;
        client = nullptr;
    }
}

void Pages::GameWindowScene::_load_with_context()
{
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
    
    map = LoadTextureFromImage(map_image);
}

void Pages::GameWindowScene::_cleanup_with_context()
{
    UnloadTexture(player_spritesheet);
    delete player_idle;
    delete player_moving;
    delete gun_idle;
    delete gun_shot;
    gun_controllers.clear();
    player_controllers.clear();
}

void Pages::GameWindowScene::game_update_callback(const Game::GameState server_gs, size_t size)
{
    game_state = server_gs;
}

void Pages::GameWindowScene::logic_update()
{
    set_curr_frame();
    client->send_frame(&curr_frame);
    made_frames.push_back(curr_frame);
    for(size_t i = game_state.player_vector[player_id].last_frame_processed_num+1; i<=curr_frame.frame_num; i++)
    {
        game_state.apply_frame(made_frames[i]);
        set_tracker(made_frames[i].delta_time);
    }
}

void Pages::GameWindowScene::set_curr_frame()
{
    curr_frame.a_pressed = IsKeyDown(KEY_A);
    curr_frame.s_pressed = IsKeyDown(KEY_S);
    curr_frame.d_pressed = IsKeyDown(KEY_D);
    curr_frame.w_pressed = IsKeyDown(KEY_W);
    curr_frame.lmb_pressed = IsKeyDown(MOUSE_BUTTON_LEFT);
    curr_frame.delta_time = GetFrameTime();
    curr_frame.frame_num++;
    curr_frame.player_id = player_id;
    curr_frame.mouse_position_screen = GetMousePosition();
}

void Pages::GameWindowScene::set_tracker(float delta_time)
{
    // Targeted radial distance (mouse distance)
    crosshair_data.mouse_distance = sqrt(((curr_frame.mouse_position_screen.x-(Maps::maps[0].tiles_in_screen_x*Maps::maps[0].tile_width_units)/2)*(curr_frame.mouse_position_screen.x-(Maps::maps[0].tiles_in_screen_x*Maps::maps[0].tile_width_units)/2) + (curr_frame.mouse_position_screen.y-(Maps::maps[0].tiles_in_screen_y*Maps::maps[0].tile_width_units)/2)*(curr_frame.mouse_position_screen.y-(Maps::maps[0].tiles_in_screen_y*Maps::maps[0].tile_width_units)/2)));

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
    crosshair_data.tracker_distance = std::max((float)crosshair_data.tracker_distance, static_cast<float>(Game::Data::gun_types[game_state.player_vector[player_id].gun_type].width));

    // Update coordinates
    crosshair_data.tracker_position.x = crosshair_data.tracker_distance*cos(game_state.player_vector[player_id].gun_angle);
    crosshair_data.tracker_position.y = crosshair_data.tracker_distance*sin(game_state.player_vector[player_id].gun_angle);
}

void Pages::GameWindowScene::CrosshairData::init()
{
    tracker_position = Vector2();
    tracker_radial_speed = 500;
    tracker_radius = 10;
    tracker_distance = 0;
}
