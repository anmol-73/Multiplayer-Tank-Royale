#include "main_window.hpp"

void Pages::MainWindowScene::_update()
{
    if (WindowShouldClose()){
        SceneManagement::SceneManager::quit();
        return;
    }

    BeginDrawing();{
        ClearBackground(VIOLET);
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        Color start_color = ui_state.start_button_is_hovered? Color{152, 141, 242, 255} : Color{102, 91, 242, 255};

        Rectangle start_button_bounds = Scamlib::Draw::textbox(
            {
                .content = "START!",
                .position = {
                    ((float)GetScreenWidth()) / 2, 
                    ((float)GetScreenHeight()) / 2
                },
                .translate = {-0.5f, -0.5f},
                .color = WHITE,
            }, {
                .position = {
                    ((float)GetScreenWidth()) / 2, 
                    ((float)GetScreenHeight()) / 2
                },
                .translate = {
                    -0.5, 
                    -0.5
                },
                .fill = start_color,
                .border = {
                    .width = 2,
                    .color = WHITE
                }
            },
            {20, 15}
        );

        if (CheckCollisionPointRec(GetMousePosition(), start_button_bounds)){
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            ui_state.start_button_is_hovered = true;
        } else{
            ui_state.start_button_is_hovered = false;
        }
        
        if (ui_state.start_button_is_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            ui_state.start_button_is_hovered = false;
            //SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::COLLISION_TESTING, true);
        }
    }
    EndDrawing();
}

void Pages::MainWindowScene::_loading_update() { return _update(); }