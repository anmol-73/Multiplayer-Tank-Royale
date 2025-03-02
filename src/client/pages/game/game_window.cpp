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
}

void Pages::GameWindowScene::_cleanup()
{ 
    if (client != nullptr){
        client->stop();
        client_worker.await();
        delete client;
        client = nullptr;
    }
}

void Pages::GameWindowScene::_load_with_context()
{

}

void Pages::GameWindowScene::_cleanup_with_context()
{

}

void Pages::GameWindowScene::game_update_callback(const Game::GameState server_gs, size_t size)
{
    game_state = server_gs;
}

void Pages::GameWindowScene::logic_update()
{
    set_curr_frame();
    made_frames.push_back(curr_frame);
    for(size_t i = game_state.player_vector[player_id].last_frame_processed_num+1; i<=curr_frame.frame_num; i++)
    {
        game_state.apply_frame(made_frames[i]);
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

// TODO
/**
 * encode decode thing
 * server
 * drawing
 */
