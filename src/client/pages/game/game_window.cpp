#include "game_window.hpp"

double td = 0;

void Pages::GameWindowScene::_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }
    
    if (client->is_game_over() or (IsKeyPressed(KEY_ESCAPE) and IsKeyPressed(KEY_LEFT_CONTROL))){
        std::unique_lock<std::mutex> lk(gs_mutex);
        std::vector<std::pair<std::string, int>> leaderboard;
        for (size_t i = 0; i < prepared_args.player_details.size(); i++) {
            if (!game_state.player_vector[prepared_args.player_details[i].id].exists) continue;
            leaderboard.push_back(std::make_pair(prepared_args.player_details[i].name, game_state.player_vector[prepared_args.player_details[i].id].score));
        }
        std::sort(leaderboard.begin(), leaderboard.end(), [](const auto& a, const auto& b) { return a.second > b.second;});
        
        size_t sz = leaderboard.size();
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::GOVER, &sz, 0);
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::GOVER, leaderboard.data(), 1);
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::GOVER);
        return;
    }

    if (!game_state.player_vector[prepared_args.pi.id].is_alive){
        ui.show();
        ui.allow_respawn(client->allow_respawn());
    } else{
        ui.hide();
    }

    BeginDrawing();{
        ClearBackground({0x1e, 0x1e, 0x1e, 0xff});

        std::unique_lock<std::mutex> lk(gs_mutex);
        renderer.draw(game_state, prepared_args.pi.id, prepared_args.player_details);
        renderer.draw_leaderboard(game_state, prepared_args.player_details);

        ui.draw();
    }
    EndDrawing();
    ui.poll_events();
    
    if (ui.should_respawn()){
        Game::TypeSelection selection;
        selection.gun_type = ui.turret_idx;
        selection.tank_type = ui.tank_idx;
        selection.player_id = prepared_args.pi.id;
        client->send_selection(selection);
        renderer.prepare(prepared_args.settings.map, selection.tank_type);
    }

    {
        std::unique_lock<std::mutex> lk(gs_mutex);
        if (game_state.player_vector[prepared_args.pi.id].is_alive){
            logic_update();    
        }
    }
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
            .content = "Joining Game...   :)",
            .font_size = Global::rem * 2,
            .font_color = {0, 0, 0, 0x60}
        });
    }
    EndDrawing();
}

void Pages::GameWindowScene::_prepare(const void *msg, size_t command)
{
    switch(command){
        case 0: {
            assert(msg != nullptr);
            prepared_args.address = *static_cast<const Communication::Address*>(msg);
            break;
        }
        
        case 1: {
            assert(msg != nullptr);
            prepared_args.player_details_size = *static_cast<const size_t*>(msg);
            break;
        }
        
        case 2: {
            assert(msg != nullptr);
            auto ptr = static_cast<const Communication::Room::PlayerDetail*>(msg);
            prepared_args.player_details.assign(ptr, ptr + prepared_args.player_details_size);
            break;
        }
        
        case 3: {
            assert(msg != nullptr);
            prepared_args.settings = *static_cast<const Communication::Room::RoomSettings*>(msg);
            break;
        }

        case 4: {
            assert(msg != nullptr);
            prepared_args.pi = *static_cast<const Communication::Game::PlayerIdentification*>(msg);
            break;
        }

        default: assert(false);
    }
}

void Pages::GameWindowScene::_load()
{
    ui.load_async();
    renderer.load_async();
    
    client = new ServiceConsumers::GameClient([this](const Game::GameState server_gs, size_t size)->void{game_update_callback(server_gs, size);});
    std::string error = client->connect(prepared_args.address);
    
    if (error.size() > 0){
        SceneManagement::SceneManager::prepare_scene(SceneManagement::SceneName::SPLASH, error.c_str(), 0);
        SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::SPLASH, 0);
        return;
    }
    
    game_state = {};
    game_state.init_game_state(prepared_args.settings.map);
    made_frames.clear();

    client->identify(prepared_args.pi);
    
    client_worker.accomplish([this](auto _){
        client->start();
    });
    
    renderer.crosshair_data.init();

    renderer.prepare(prepared_args.settings.map, 0);
    curr_frame.frame_num = 0;   
}

void Pages::GameWindowScene::_cleanup()
{
    ui.cleanup_async();
    renderer.cleanup_async();
    if (client != nullptr){
        client->stop();
        client_worker.await();
        delete client;
        client = nullptr;
    }
}

void Pages::GameWindowScene::_load_with_context()
{
    ui.load_sync();
    renderer.load_sync();
}

void Pages::GameWindowScene::_cleanup_with_context()
{
    ui.cleanup_sync();
    renderer.cleanup_sync();
}

void Pages::GameWindowScene::game_update_callback(const Game::GameState server_gs, size_t size)
{
    // auto t2 = game_state.curtime();
    std::unique_lock<std::mutex> lk(gs_mutex);
    game_state = server_gs;
    // std::cout << "GS DELAY: " << t2 - td << std::endl;
    // td = t2;
    
}

void Pages::GameWindowScene::logic_update()
{
    set_curr_frame();
    client->send_frame(&curr_frame);

    made_frames.push_back(curr_frame);
    
    // auto t1 = game_state.curtime();
    // int time = curr_frame.frame_num - game_state.player_vector[prepared_args.pi.id].last_frame_processed_num;
    for(size_t i = game_state.player_vector[prepared_args.pi.id].last_frame_processed_num+1; i<=curr_frame.frame_num; i++)
    {
        game_state.apply_frame(made_frames[i]);
    }
    // auto t2 = game_state.curtime();
    // std::cout << t2 - t1 << ' ' << time << std::endl;
}

void Pages::GameWindowScene::set_curr_frame()
{
    curr_frame.a_pressed = IsKeyDown(KEY_A);
    curr_frame.s_pressed = IsKeyDown(KEY_S);
    curr_frame.d_pressed = IsKeyDown(KEY_D);
    curr_frame.w_pressed = IsKeyDown(KEY_W);
    curr_frame.lmb_pressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    curr_frame.delta_time = GetFrameTime();
    curr_frame.frame_num = made_frames.size();
    curr_frame.player_id = prepared_args.pi.id;
    Vector2 temp = GetMousePosition();
    renderer.crosshair_data.mouse_position = {temp.x, GetScreenHeight()-temp.y};
    curr_frame.mouse_position_screen = renderer.camera.descale(renderer.crosshair_data.mouse_position);
}
