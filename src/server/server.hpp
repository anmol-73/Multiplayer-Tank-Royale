#ifndef HEADER_SERVER
#define HEADER_SERVER

#include <stdint.h>
#include <stdexcept>
#include <enet/enet.h>

class Server{
    public:
        Server(uint32_t port);
        ~Server();
	    void send(const void* buffer, size_t size, ENetPeer* peer, _ENetPacketFlag flag);
	    void disconnect(ENetPeer* peer);

        ENetHost* serverHost;
	    ENetEvent event;
	    ENetAddress address;

private:
};


#endif