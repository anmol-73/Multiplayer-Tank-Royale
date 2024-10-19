#ifndef HEADER_COMMUNICATION_CLIENT
#define HEADER_COMMUNICATION_CLIENT

#include <iostream>

#if defined(_WIN32)           
	#define NOGDI             // All GDI defines and routines
	#define NOUSER            // All USER defines and routines
#endif
typedef struct tagMSG *LPMSG;
#include <enet/enet.h>

#if defined(_WIN32)           // raylib uses these names as function parameters
	#undef near
	#undef far
    #undef ABSOLUTE
#endif

#include <memory>
#include <stdexcept>
#include <map>
#include "networking.hpp"

namespace Communication
{
    struct Client{
    public:
        /**
         * Connect to the server
         * @returns succesful, reason of failure
         */
        std::pair<bool, std::string> connect(std::string server_address, const bool& cancel_requested);

        /**
         * Cleans up all resources used by the host
         */
        void cleanup();

        /**
         * Starts listening and handling connections
         * NOTE: This will block, you probably need to run this on a new thread
         */
        void run();

        /**
         * Stops the server from running
         */
        void stop();

    protected:
        /**
         * Destroys the current host if it has been created
         */
        void destroy_host();

        /**
         * Overload this to do additional logics (perhaps check if the server has actually accepted you)
         * when a connection has been established with the server
         */
        virtual std::pair<bool, std::string> handle_established_connection(const bool& cancel_requested);

        /**
         * Overload this to do some logic when the serveer sends a message
         * NOTE: This provides the decoded message (Refer to `common/networking/helper.hpp`)
         */
        virtual void handle_message(size_t type, void* message);

        /**
         * Overload this to do some extra logic when the server has started
         */
        virtual void on_run();
        /**
         * Overload this to do some extra logic when the server has finished running
         */
        virtual void on_stop();

        /**
         * Sends a command (along with the message data) to the server
         */
        void send(size_t command, void* data, size_t data_size, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
        
        /**
         * Sends a command to the server
         */
        void send(size_t command, ENetPacketFlag flag = ENET_PACKET_FLAG_RELIABLE);
        

        ENetAddress address;
        ENetHost* host = nullptr;
        ENetPeer* peer = nullptr;
    
    private:
        volatile bool is_running = false;
    };
    
} // namespace Communication


#endif