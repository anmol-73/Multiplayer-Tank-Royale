#ifndef HEADER_LAYOUT
#define HEADER_LAYOUT

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
                // TODO: Add support for border radius?
                // (One of the initial commits has the struct definition to store radius information it doesn't have an implementation yet)
            };
            enum SizeMode{
                SCREEN_W, // Relative to parent width
                SCREEN_H, // Relative to parent height
                SELF_H, // Relative to self height
                SELF_W, // Relative to self width
                ABSOLUTE // Pixels
            };
            using CalculationMode = std::array<SizeMode, 2>;
            struct Box{
                CalculationMode position_mode = {SizeMode::SCREEN_W, SizeMode::SCREEN_H};
                Vector2 position = {0}; // The position to place the origin of the rect

                CalculationMode size_mode = {SizeMode::SCREEN_W, SizeMode::SCREEN_H};
                Vector2 size = {0}; // The size of the box
                
                CalculationMode origin_mode = {SizeMode::SELF_W, SizeMode::SELF_H};
                Vector2 origin = {0}; // Specifies the origin of the rect

                Color fill = {255, 255, 255, 0}; // The color to fill the box with

                BorderParameters border;
            };

            struct Text{
                std::string content; // The text content
                Font* font; // The font (uses `Global::Rendering::Fonts::main` if set to null)
                bool sdf = true;
                float font_size = 32; // Font size
                Color font_color = GRAY; // Font color
                
                CalculationMode position_mode = {SizeMode::SCREEN_W, SizeMode::SCREEN_H};
                Vector2 position = {0}; // The position to place the origin of the rect
                
                CalculationMode origin_mode = {SizeMode::SELF_W, SizeMode::SELF_H};
                Vector2 origin = {0}; // Specifies the origin of the rect
               
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
                
                CalculationMode position_mode = {SizeMode::SCREEN_W, SizeMode::SCREEN_H};
                Vector2 position = {0}; // The position to place the origin of the rect

                CalculationMode rect_origin_mode = {SizeMode::SELF_W, SizeMode::SELF_H};
                Vector2 rect_origin = {0}; // Specifies the origin of the rect
                
                Color fill = {255, 255, 255, 0};
                BorderParameters border;
                
                CalculationMode text_origin_mode = {SizeMode::SELF_W, SizeMode::SELF_H};
                Vector2 text_origin = {0}; // Specifies the origin of the text wrt to the rect
                
                float spacing = 1; // The space between letters
                float line_space = 1.0f; // The vertical padding between lines of text (As a percentage of height of the text)
            };
        } // namespace DrawParameters
    } // namespace UI    
} // namespace DragonLib

#endif