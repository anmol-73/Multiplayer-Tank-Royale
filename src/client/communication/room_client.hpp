#ifndef HEADER_COMMUNICATION_ROOM_CLIENT
#define HEADER_COMMUNICATION_ROOM_CLIENT

#include <enet/enet.h>
#include <memory>
#include <stdexcept>
#include <map>
#include <vector>
#include "client.hpp"

namespace Communication
{
    struct RoomClient: public Client{
    /**
     * Overload this to do additional logics (perhaps check if the server has actually accepted you)
     * when a connection has been established with the server
     */
    virtual std::pair<bool, std::string> handle_established_connection();

    private:
        size_t id;
    };
} // namespace Communication


#endif