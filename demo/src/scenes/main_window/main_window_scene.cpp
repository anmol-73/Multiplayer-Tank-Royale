#include "main_window_scene.hpp"

void Scenes::MainWindowScene::update()
{
    // The main screen doesn't really have any loading to do, its always ready only :)
    if (WindowShouldClose()){
        SceneManagement::quit();
        return;
    }
    BeginDrawing();{
        ClearBackground({80, 80, 80, 255});
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        Color start_color = ui_state.start_button_is_hovered? Color{152, 141, 242, 255} : Color{102, 91, 242, 255};
        Color other_color = ui_state.other_button_is_hovered? Color{152, 141, 242, 255} : Color{102, 91, 242, 255};
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

        Rectangle other_button_bounds = Scamlib::Draw::textbox(
            {
                .content = "OTHER",
                .position = {
                    ((float)GetScreenWidth()) / 2, 
                    ((((float)GetScreenHeight()) / 2) + 100)
                },
                .translate = {-0.5f, -0.5f},
                .color = WHITE,
            }, {
                .position = {
                    ((float)GetScreenWidth()) / 2, 
                    ((((float)GetScreenHeight()) / 2) + 100)
                },
                .translate = {
                    -0.5f, 
                    -0.5f
                },
                .fill = other_color,
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

        if (CheckCollisionPointRec(GetMousePosition(), other_button_bounds)){
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            ui_state.other_button_is_hovered = true;
        } else{
            ui_state.other_button_is_hovered = false;
        }
        
        if (ui_state.start_button_is_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            ui_state.start_button_is_hovered = false;
            SceneManagement::load_scene(SceneManagement::SceneName::GAME_WINDOW);
        }

        if (ui_state.other_button_is_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            ui_state.other_button_is_hovered = false;
            SceneManagement::load_scene(SceneManagement::SceneName::NEW_GAME_WINDOW);
        }
    }
    EndDrawing();
    return;
}
