#include "Server.hpp"

Server::Server(uint32_t port) {
	address.host = ENET_HOST_ANY;
	address.port = port;

	if (enet_initialize() != 0) {
		throw std::runtime_error("cannot initialize enet");
	}

	serverHost = enet_host_create(&address,
		32,   // number of clients
		2,    // number of channels
		0,    // assume any amount of incoming bandwidth 
		0     // assume any amount of outgoing bandwidth
	);
	if (serverHost == NULL) {
		throw std::runtime_error("An error occurred while trying to create an ENet server host");
	}
}

Server::~Server() {
	enet_host_destroy(serverHost);
	enet_deinitialize();
}



void Server::send(const void* buffer, size_t size, ENetPeer* peer, _ENetPacketFlag flag) {
	ENetPacket* packet = enet_packet_create(buffer, size + 1, flag);
	enet_peer_send(peer, 0, packet);
}

void Server::disconnect(ENetPeer* peer) {
}