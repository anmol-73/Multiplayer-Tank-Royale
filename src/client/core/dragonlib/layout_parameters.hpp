#ifndef H_DLIB_LAYOUT_PARAMS
#define H_DLIB_LAYOUT_PARAMS

#include <raylib.h>
#include <string>
#include <array>

namespace DragonLib
{
    namespace UI
    {
        namespace DrawParameters
        {
            struct BorderParameters{
                float width = 0; // The width of the border
                Color color = BLACK; // The color of the border
                float offset = 0; // The offset around the box with which the border is drawn
                
                // FUTURE: Add support for border radius?
                // (One of the initial commits has the struct definition to store radius information it doesn't have an implementation yet)
            };
            enum SizeMode{
                SCREEN_W, // Relative to parent width
                SCREEN_H, // Relative to parent height
                SELF_H, // Relative to self height
                SELF_W, // Relative to self width
                ABSOLUTE // Pixels
            };
            struct Measurement{
                Vector2 value;
                std::array<SizeMode, 2> mode;
            };

            struct Box{
                Measurement position = { // The position to place the origin of the rect
                    .value = {0.5, 0.5},
                    .mode = {SizeMode::SCREEN_W, SizeMode::SCREEN_H}
                };

                Measurement size = { // The size of the box
                    .value = {0.1, 0.1},
                    .mode = {SizeMode::SCREEN_W, SizeMode::SCREEN_H}
                };
                
                Measurement origin = { // Specifies the origin of the rect
                    .value = {0.5, 0.5},
                    .mode = {SizeMode::SELF_W, SizeMode::SELF_H}
                };

                Measurement padding = { // Padding to add on the left and the right. NOTE: SELF is wrt the content rect!
                    .value = {0},
                    .mode = {SizeMode::SELF_W, SizeMode::SELF_H}
                };

                Color fill = {255, 255, 255, 0}; // The color to fill the box with

                BorderParameters border;
            };

            struct Text{
                std::string content; // The text content
                Font* font; // The font (uses `Global::Rendering::Fonts::main` if set to null)
                bool sdf = true;
                float font_size = 32; // Font size
                Color font_color = GRAY; // Font color
                
                Measurement position = { // The position to place the origin of the rect
                    .value = {0.5, 0.5},
                    .mode = {SizeMode::SCREEN_W, SizeMode::SCREEN_H}
                };
                
                Measurement origin = { // Specifies the origin of the rect
                    .value = {0.5, 0.5},
                    .mode = {SizeMode::SELF_W, SizeMode::SELF_H}
                };
               
                float wrap_width = 0; // Set to 0 if no wrapping to be done
                
                float spacing = 1; // The space between letters
                float line_space = 1.0f; // The vertical padding between lines of text (As a percentage of height of the text)
            };

            struct TextBox{
                std::string content; // The text content
                Font* font; // The font (uses `Global::Rendering::Fonts::main` if set to null)
                bool sdf = true;
                float font_size = 32; // Font size
                Color font_color = GRAY; // Font color                
                
                Measurement position = { // The position to place the origin of the rect
                    .value = {0.5, 0.5},
                    .mode = {SizeMode::SCREEN_W, SizeMode::SCREEN_H}
                };

                Measurement rect_origin = { // Specifies the origin of the rect
                    .value = {0.5, 0.5},
                    .mode = {SizeMode::SELF_W, SizeMode::SELF_H}
                };
                
                Color fill = {255, 255, 255, 0};
                BorderParameters border;

                Measurement padding = { // Padding to add on the left and the right. NOTE: SELF is wrt the content rect!
                    .value = {0},
                    .mode = {SizeMode::SELF_W, SizeMode::SELF_H}
                };
                
                Measurement text_origin = { // Specifies the origin of the text wrt to the rect origin. NOTE: SELF is wrt the content rect!
                    .value = {0},
                    .mode = {SizeMode::SELF_W, SizeMode::SELF_H}
                };
                
                float spacing = 1; // The space between letters
                float line_space = 1.0f; // The vertical padding between lines of text (As a percentage of height of the text)
            };
        } // namespace DrawParameters
    } // namespace UI    
} // namespace DragonLib

#endif