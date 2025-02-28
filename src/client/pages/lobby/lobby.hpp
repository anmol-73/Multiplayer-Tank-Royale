#ifndef H_PAGES_LOBBY
#define H_PAGES_LOBBY

#include "core/scene_management.hpp"
#include "lobby_ui.hpp"
#include "lobby_client.hpp"
#include "misc/task.hpp"

namespace Pages
{
    class LobbyScene: public SceneManagement::Scene{
    public:
        virtual void _update();
        virtual void _loading_update();

    protected:
        /** Expects a pointer to an Address! */
        virtual void _prepare(const void *address);
        virtual void _load();
        virtual void _cleanup();
        virtual void _load_with_context();
        virtual void _cleanup_with_context();

    private:
        LobbyUI ui;
        Communication::Address address = {};

        ServiceConsumers::LobbyClient *client = nullptr;

        Utils::Task client_worker;
    };
} // namespace Pages

#endif