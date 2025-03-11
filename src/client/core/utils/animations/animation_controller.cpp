#include "animation_controller.hpp"

void Utils::Animation::draw(float time, Rectangle draw_box){}

void Utils::AnimationController::reset()
{
    counter = 0;
    current_iteration_count = 0;
}

void Utils::AnimationController::draw(float delta_time, Rectangle draw_box)
{
    counter += delta_time;
    while (counter >= animations[current_anim]->duration){
        counter -= animations[current_anim]->duration;
        ++current_iteration_count;
    }

    animations[current_anim]->draw(counter, draw_box);
}

void Utils::AnimationController::play(size_t anim_id, bool reset_if_already_playing)
{
    if (anim_id == current_anim && !reset_if_already_playing) return;

    if (anim_id >= animations.size()){
        throw std::runtime_error("ERROR! Tried to play unregistered animation");
    }

    reset();
    current_anim = anim_id;
}

size_t Utils::AnimationController::register_animation(Animation *anim)
{
    animations.emplace_back(anim);
    return animations.size() - 1;
}
