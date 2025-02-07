#ifndef H_DLIB_IMAGE
#define H_DLIB_IMAGE

#include <raylib.h>
#include <string>

namespace DragonLib
{
    struct DImage{
    public:
        DImage(const std::string& path_);
        DImage();
        
        void load_im();
        void unload_im();
        void load_tex();
        void unload_tex();
        std::string path;
        Image im;
        Texture tex;
    };
} // namespace DragonLib

#endif