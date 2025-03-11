
#ifndef H_UTILS_ANIMATION_CONTROLLER
#define H_UTILS_ANIMATION_CONTROLLER

#include <vector>
#include <memory>
#include <stdexcept>
#include <raylib.h>

namespace Utils
{
    struct Animation{
    public:
        Animation(float duration): duration(duration) {}

        const float duration;
        void draw(float time, Rectangle draw_box);
    };

    struct AnimationController{
    private:
        /**
         * ID of the animation currently playing
        */
       size_t current_anim = 0;

        /**
         * All animation used by the animation controller
         */
        std::vector<std::unique_ptr<Animation>> animations;
        
        /**
         * Basically the timer
         */
        float counter = 0;

        /**
         * Resets the controller back to original state (Used when playing a new animation)
         */
        void reset();

    public:
        /**
         * The number of times the currently playing animation has looped
         */
        size_t current_iteration_count = 0;

        /**
         * To be called every frame, updates the animation timer and the animation frame
         */
        void draw(float delta_time, Rectangle draw_box);
        
        /**
         * Starts playing the animation corresponding to the given idx
         */
        void play(size_t anim_id, bool reset_if_already_playing = true);

        /**
         * Registers the animation and returns the idx
         */
        size_t register_animation(Animation *anim);
    };
} // namespace Utils

#endif