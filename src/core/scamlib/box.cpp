#include "box.hpp"
#include <iostream>
#include <rlgl.h>

Rectangle Scamlib::Draw::box(BoxDrawParameters params)
{
    params.position.x += params.translate.x * params.size.x;
    params.position.y += params.translate.y * params.size.y;

    if (params.border.radius.bottom_left == 0 && params.border.radius.bottom_right == 0 && params.border.radius.top_left == 0 && params.border.radius.top_right == 0){
        return __box_no_radius(params);
    } else{
        return __box_with_radius(params);
    }
}

Rectangle Scamlib::Draw::__box_no_radius(BoxDrawParameters params)
{
    if (params.fill.a != 0){
        DrawRectangleV(params.position, params.size, params.fill);
    }
    if (params.border.width != 0 && params.border.color.a != 0){
        DrawRectangleLinesEx({
            .x = params.position.x,
            .y = params.position.y,
            .width = params.size.x,
            .height = params.size.y,
        }, params.border.width, params.border.color);
    }

    return {
        params.position.x, params.position.y,
        params.size.x, params.size.y
    };
}

Rectangle Scamlib::Draw::__box_with_radius(BoxDrawParameters params)
{
    // TODO: Do rect with border radius
    // https://www.shadertoy.com/view/WtdSDs
    // Look into Triangle Fan
    return {
        params.position.x, params.position.y,
        params.size.x, params.size.y
    };
}
