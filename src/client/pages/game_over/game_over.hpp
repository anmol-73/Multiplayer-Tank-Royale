#ifndef H_PAGES_GOVER
#define H_PAGES_GOVER

#include "core/scene_management.hpp"
#include "game_over_ui.hpp"
#include "communication/structs.hpp"

namespace Pages
{
    class GameOverScene: public SceneManagement::Scene{
    public:
        virtual void _update();
        virtual void _loading_update();

    protected:
        virtual void _prepare(const void *msg, size_t command);
        virtual void _load();
        virtual void _cleanup();
        virtual void _load_with_context();
        virtual void _cleanup_with_context();

    private:
        struct {
            size_t list_size;
        } pi;
        GameOverUI ui;
    };
} // namespace Pages

#endif