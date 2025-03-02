#ifndef H_PAGES_ROOM
#define H_PAGES_ROOM

#include "core/scene_management.hpp"
#include "room_ui.hpp"
#include "room_client.hpp"
#include "misc/task.hpp"
#include "maps/maps.hpp"

namespace Pages
{
    class RoomScene: public SceneManagement::Scene{
    public:
        virtual void _update();
        virtual void _loading_update();

    protected:
        /** Expects a pointer to chunk with an Address and a RoomDetail! */
        virtual void _prepare(const void *msg, size_t command);
        virtual void _load();
        virtual void _cleanup();
        virtual void _load_with_context();
        virtual void _cleanup_with_context();

    private:
        RoomUI ui;
        
        ServiceConsumers::RoomClient *client = nullptr;
        Utils::Task client_worker;

        Communication::Address address = {};
        Communication::Lobby::RoomDetail room_detail = {};
    };
} // namespace Pages

#endif