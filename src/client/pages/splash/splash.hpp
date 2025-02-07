#ifndef H_PAGES_SPLASH
#define H_PAGES_SPLASH

#include "core/scene_management.hpp"
#include "splash_ui.hpp"

namespace Pages
{
    class SplashScene: public SceneManagement::Scene{
    public:
        virtual void _update();
        virtual void _loading_update();

    protected:
        virtual void _load();
        virtual void _cleanup();
        virtual void _load_with_context();
        virtual void _cleanup_with_context();

    private:
        SplashUI ui;
    };
} // namespace Pages

#endif