#include "layout_calculation.hpp"

Vector2 DragonLib::Utils::Calculation::resolve_measurement(UI::DrawParameters::Measurement measurement, Vector2 element_size)
{
    Vector2 out = {0};
    switch (measurement.mode[0])
    {
    case UI::DrawParameters::SizeMode::ABSOLUTE:
        out.x = measurement.value.x;
        break;
    case UI::DrawParameters::SizeMode::SCREEN_H:
        out.x = measurement.value.x * GetScreenHeight();
        break;
    case UI::DrawParameters::SizeMode::SCREEN_W:
        out.x = measurement.value.x * GetScreenWidth();
        break;
    case UI::DrawParameters::SizeMode::SELF_H:
        out.x = measurement.value.x * element_size.y;
        break;
    case UI::DrawParameters::SizeMode::SELF_W:
        out.x = measurement.value.x * element_size.x;
        break;

    default:        
        // Unreachable
        assert(false);
    }

    switch (measurement.mode[1])
    {
    case UI::DrawParameters::SizeMode::ABSOLUTE:
        out.y = measurement.value.y;
        break;
    case UI::DrawParameters::SizeMode::SCREEN_H:
        out.y = measurement.value.y * GetScreenHeight();
        break;
    case UI::DrawParameters::SizeMode::SCREEN_W:
        out.y = measurement.value.y * GetScreenWidth();
        break;
    case UI::DrawParameters::SizeMode::SELF_H:
        out.y = measurement.value.y * element_size.y;
        break;
    case UI::DrawParameters::SizeMode::SELF_W:
        out.y = measurement.value.y * element_size.x;
        break;

    default:        
        // Unreachable
        assert(false);
    }
    return out;
}
