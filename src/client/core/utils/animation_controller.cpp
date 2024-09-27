#include "animation_controller.hpp"

void Utils::Animation::add_keyframe(float duration, Rectangle frame_rectangle){
            float end = duration;
            if (keyframes.size() > 0){
                end += keyframes.back().first;
            }

            keyframes.push_back({end, frame_rectangle});
        }

void Utils::AnimationController::reset(){
            Animation* anim = animations[current_anim].get();
            anim->frame_idx = 0;
            counter = 0;
            current_iteration_count = 0;
        }

std::pair<Texture2D*, Rectangle*> Utils::AnimationController::get_sprite(){
            Animation* anim = animations[current_anim].get();
            return {anim->spritesheet, &(anim->keyframes[anim->frame_idx].second)};
        }

void Utils::AnimationController::update(float delta_time)
{
    counter += delta_time;
    Animation* anim = animations[current_anim].get();
    while (counter >= anim->keyframes[anim->frame_idx].first){
        anim->frame_idx += 1;
        if (anim->frame_idx >= anim->keyframes.size()){
            anim->frame_idx = 0;
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

            animations.push_back(std::unique_ptr<Animation>(anim));
            return animations.size() - 1;
        }