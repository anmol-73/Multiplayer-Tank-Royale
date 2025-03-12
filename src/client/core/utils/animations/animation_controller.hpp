
#ifndef H_UTILS_ANIMATION_CONTROLLER
#define H_UTILS_ANIMATION_CONTROLLER

#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <raylib.h>
#include <list>

namespace Utils
{
    struct Animation{
    public:
        Animation(float duration): duration(duration) {}

        const float duration;
        virtual void draw(float time, Rectangle draw_box, float angle);
    };
    struct AAnimation: public Animation{
    public:
        AAnimation(float duration, std::function<void(float, Rectangle, float)> drawer): Animation(duration), drawer(drawer) {}

        const std::function<void(float, Rectangle, float)> drawer;
        void draw(float time, Rectangle draw_box, float angle);
    };
    
    struct EffectAnimationController{
        /**
         * All animation used by the animation controller
         */
        std::vector<std::unique_ptr<Animation>> animations;

        struct EffectAnimationData{
            std::function<std::pair<Rectangle, float>(float)> draw_box_provider;

            float counter;
            size_t animation_id;
        };
      
        std::list<EffectAnimationData> active_effects;

    public:
        /**
         * To be called every frame, updates the animation timer and the animation frame
         */
        void draw(float delta_time);
        
        /**
         * Starts playing the animation corresponding to the given idx
         */
        void play(size_t anim_id, std::function<std::pair<Rectangle, float>(float)> draw_box_provider);

        /**
         * Registers the animation and returns the idx
         */
        size_t register_animation(Animation *anim);
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
        AnimationController(): current_anim(0){}
        /**
         * The number of times the currently playing animation has looped
         */
        size_t current_iteration_count = 0;

        /**
         * To be called every frame, updates the animation timer and the animation frame
         */
        void draw(float delta_time, Rectangle draw_box, float angle);
        
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