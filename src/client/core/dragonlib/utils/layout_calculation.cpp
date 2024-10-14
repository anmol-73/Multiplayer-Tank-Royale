#include "layout_calculation.hpp"

Vector2 DragonLib::Utils::Calculation::resolve_item_units(Vector2 item, UI::DrawParameters::CalculationMode mode, Vector2 element_size)
{
    Vector2 out = {0};
    switch (mode[0])
    {
    case UI::DrawParameters::SizeMode::ABSOLUTE:
        out.x = item.x;
        break;
    case UI::DrawParameters::SizeMode::SCREEN_H:
        out.x = item.x * GetScreenHeight();
        break;
    case UI::DrawParameters::SizeMode::SCREEN_W:
        out.x = item.x * GetScreenWidth();
        break;
    case UI::DrawParameters::SizeMode::SELF_H:
        out.x = item.x * element_size.y;
        break;
    case UI::DrawParameters::SizeMode::SELF_W:
        out.x = item.x * element_size.x;
        break;

    default:        
        // Unreachable
        assert(false);
    }

    switch (mode[1])
    {
    case UI::DrawParameters::SizeMode::ABSOLUTE:
        out.y = item.y;
        break;
    case UI::DrawParameters::SizeMode::SCREEN_H:
        out.y = item.y * GetScreenHeight();
        break;
    case UI::DrawParameters::SizeMode::SCREEN_W:
        out.y = item.y * GetScreenWidth();
        break;
    case UI::DrawParameters::SizeMode::SELF_H:
        out.y = item.y * element_size.y;
        break;
    case UI::DrawParameters::SizeMode::SELF_W:
        out.y = item.y * element_size.x;
        break;

    default:        
        // Unreachable
        assert(false);
    }
    return out;
}
