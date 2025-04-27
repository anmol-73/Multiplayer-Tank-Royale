#include "animation_controller.hpp"
#include <iostream>

void Utils::Animation::draw(float time, Rectangle draw_box, float angle){}

void Utils::AnimationController::reset()
{
    counter = 0;
    current_iteration_count = 0;
}

void Utils::AnimationController::draw(float delta_time, Rectangle draw_box, float angle)
{
    counter += delta_time;
    while (counter >= animations[current_anim]->duration){
        counter -= animations[current_anim]->duration;
        ++current_iteration_count;
    }

    animations[current_anim]->draw(counter, draw_box, angle);
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

void Utils::EffectAnimationController::draw(float delta_time)
{
    for (auto it = active_effects.begin(); it != active_effects.end();){
        it->counter += delta_time;
        std::cout << it->animation_id << ' ' << animations[0]->duration << ' ' << it->counter << std::endl;
        if (animations[it->animation_id]->duration < it->counter){
            it = active_effects.erase(it); continue;
        }
        const auto [rect, angle] = it->draw_box_provider(it->counter);
        animations[it->animation_id]->draw(it->counter, rect, angle);
        ++it;
    }
}

void Utils::EffectAnimationController::play(size_t anim_id, std::function<std::pair<Rectangle, float>(float)> draw_box_provider)
{
    active_effects.push_back(EffectAnimationData{
        .draw_box_provider = draw_box_provider,
        .counter = 0,
        .animation_id = anim_id
    });
}

size_t Utils::EffectAnimationController::register_animation(Animation *anim)
{
    animations.emplace_back(anim);
    return animations.size() - 1;
}

void Utils::AAnimation::draw(float time, Rectangle draw_box, float angle)
{
    drawer(time, draw_box, angle);
}
