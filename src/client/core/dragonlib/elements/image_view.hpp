#ifndef H_DLIB_ELEMENT_IMAGE_VIEW
#define H_DLIB_ELEMENT_IMAGE_VIEW

#include "view.hpp"
#include "../utils.hpp"

namespace DragonLib
{
    namespace UI
    {
        namespace Elements
        {
            struct ImageView: public View{
            public:
                ImageView(DImage image, std::function<DrawParameters::Box(ImageView*)> draw_param_provider): image(image), draw_param_provider(draw_param_provider){}

                void draw();
                void load_async();
                void cleanup_async();
                void load_sync();
                void cleanup_sync();

                DImage image;
            
            private:
                std::function<DrawParameters::Box(ImageView*)> draw_param_provider;
            };

        } // namespace Elements
    } // namespace UI    
} // namespace DragonLib

#endif