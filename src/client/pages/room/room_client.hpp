#ifndef H_PAGES_ROOM_CLIENT
#define H_PAGES_ROOM_CLIENT

#include <optional>

#include "communication/protocol.hpp"
#include "service_consumer.hpp"

namespace ServiceConsumers
{
    struct RoomClient: public ServiceConsumer{
    public:
        bool is_connected();
        
    protected:
        void handle_message(Communication::Command type, const void *message, size_t size);
        void on_start();
        void on_finish();
        std::string on_connection_established();
        void handle_disconnection();
        
    private:
        bool _is_connected = false;
    };
} // namespace Pages

#endif