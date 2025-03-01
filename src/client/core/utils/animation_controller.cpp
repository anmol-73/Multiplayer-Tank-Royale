#include "animation_controller.hpp"

void Utils::Animation::add_keyframe(float duration, Rectangle frame_rectangle){
            float end = duration;
            if (keyframes.size() > 0){
                end += keyframes.back().first;
            }

            keyframes.push_back({end, frame_rectangle});
        }

void Utils::AnimationController::reset(){
            frame_idx = 0;
            counter = 0;
            current_iteration_count = 0;
        }

std::pair<Texture2D*, Rectangle*> Utils::AnimationController::get_sprite(){
            Animation* anim = animations[current_anim];
            return {anim->spritesheet, &(anim->keyframes[frame_idx].second)};
        }

void Utils::AnimationController::update(float delta_time)
{
    counter += delta_time;
    Animation* anim = animations[current_anim];
    while (counter >= anim->keyframes[frame_idx].first){
        frame_idx += 1;
        if (frame_idx >= anim->keyframes.size()){
            frame_idx = 0;
            ++current_iteration_count;
            counter -= anim->keyframes.back().first;
        }
    }
}

void Utils::AnimationController::play(size_t anim_id, bool reset_if_already_playing)
{
    if (anim_id == current_anim && !reset_if_already_playing) return;

    if (anim_id < 0 || anim_id > animations.size()){
        throw std::runtime_error("ERROR! Tried to play unregistered animation");
    }

    reset();
    current_anim = anim_id;
}

size_t Utils::AnimationController::register_animation(Animation *anim){
    if (anim->keyframes.size() == 0){
        throw std::runtime_error("ERROR! Tried to register empty animation");
    }
    animations.push_back(anim);
    return animations.size() - 1;
}

Utils::AnimationController::AnimationController() = default;