
#ifndef HEADER_ANIMATION_CONTROLLER
#define HEADER_ANIMATION_CONTROLLER

#include <vector>
#include <stdexcept>
#include <raylib.h>

namespace Utils
{
    struct Animation{

        /**
         * The texture which contains the spritesheet
         */
        Texture2D* spritesheet;

        /**
         * Pair of end time of keyframe and the rectangle on the spritesheet that needs to be shown
         */
        std::vector<std::pair<float, Rectangle>> keyframes;

        /**
         * Adds a keyframe of a specific duration to the animation
         */
        void add_keyframe(float duration, Rectangle frame_rectangle);

        /**
         * @param spritesheet The texture which contains the spritesheet
         * @param _keyframes List of (duration of frame, FrameRectangle)
         */
        Animation(Texture2D* spritesheet, std::vector<std::pair<float, Rectangle>> _keyframes): spritesheet(spritesheet), keyframes(_keyframes){
            for (size_t i = 1; i < keyframes.size(); ++i){
                keyframes[i].first += keyframes[i - 1].first;
            }
        }
    };

    struct AnimationController{
    private:
        /**
         * ID of the animation currently playing
        */

        /**
         * The current frame being shown
         */
        size_t frame_idx = 0;

        /**
         * All animation used by the animation controller
         */
        std::vector<Animation*> animations;
        
        /**
         * Basically the timer
         */
        float counter = 0;

        /**
         * Resets the controller back to original state (Used when playing a new animation)
         */
        void reset();

    public:
        size_t current_anim = 0;
        /**
         * The number of times the currently playing animation has looped
         */
        size_t current_iteration_count = 0;

        /**
         * Returns the correct texture and frame_rectangle (which can then be used to draw the sprite)
         */
        std::pair<Texture2D*, Rectangle*> get_sprite();

        /**
         * To be called every frame, updates the animation timer and the animation frame
         */
        void update(float delta_time);
        
        /**
         * Starts playing the animation corresponding to the given idx
         */
        void play(size_t anim_id, bool reset_if_already_playing = true);

        /**
         * Registers the animation and returns the idx
         */
        size_t register_animation(Animation *anim);

        AnimationController();

    };
} // namespace Utils

#endif
