#include "lobby_service_provider.hpp"

#define log(message) std::cout << "[Lobby Server] " << message << std::endl

std::vector<Communication::Lobby::RoomDetail> LobbyServiceProvider::get_room_details()
{
    using namespace Communication::Lobby;

    std::vector<RoomDetail> details; {
        details.reserve(rooms.size());
        for (const auto &[port, room]: rooms){
            RoomDetail rd; {
                strncpy(rd.name, room->name.c_str(), sizeof(RoomName));
                rd.port = port;
            }

            details.push_back(rd);
        }   
    }

    return details;
}

void LobbyServiceProvider::on_start()
{
    const size_t hostname_sz = 16;
    char hostname[hostname_sz];
    enet_address_get_host_ip(&address, hostname, hostname_sz);

    log("Listening on " << hostname << ':' << address.port);
}

void LobbyServiceProvider::on_finish()
{
    log("Stopped servicing :(");

    for (const auto& [port, rsp]: rooms){
        rsp->stop();
        log("Forcibly closed room `" << rsp->name << "`.");
    }
    for (const auto& [port, gsp]: games){
        gsp->stop();
        log("Forcibly closed game `" << gsp->name << "`.");
    }

    rooms.clear();
    games.clear();
}

void LobbyServiceProvider::update()
{
    using namespace Communication::Lobby;

    std::unique_lock<std::mutex> lock(delete_mutex);
    for (const auto v: games_to_delete) games.erase(v);
    games_to_delete.assign(0, {});
    if (rooms_to_delete.size() > 0){
        for (const auto v: rooms_to_delete) rooms.erase(v);
        auto room_details = get_room_details();
        broadcast_message(Server::ROOM_LIST, room_details.data(), sizeof(RoomDetail) * room_details.size());
    }
    rooms_to_delete.assign(0, {});
}

void LobbyServiceProvider::handle_new_client(ENetPeer *peer)
{
    using namespace Communication::Lobby;

    auto details = get_room_details();
    
    send_message(Server::ROOM_LIST, details.data(), sizeof(RoomDetail) * details.size(), peer);
    
    log("New client connected! Current active connections: " << ++connected_count);
}

void LobbyServiceProvider::handle_disconnection(ENetPeer *peer)
{
    log("Client disconnected. Current active connections: " << --connected_count);
}

void LobbyServiceProvider::handle_message(ENetPeer *peer, Communication::Command type, const void *message, size_t size)
{
    using namespace Communication::Lobby;
    
    // We do not allow more than 8 room at once!
    const size_t max_room_count = 8;

    switch (static_cast<Client>(type)){
        case Client::CREATE_ROOM: {
            auto name = static_cast<const char*>(message);
            log("Requested to create room `" << name << "`.");
            
            if (rooms.size() >= max_room_count){
                log("Rejected room creation request `" << name << "`. Maximum room count already achieved!");
                
                send_command(Server::CREATE_DENIED, peer);
                
                return;
            }

            auto rsp = new RoomServiceProvider([&](int port){
                if (rooms.count(port) == 0){
                    log("CRITICAL ERROR! Ghost room provider detected!");
                    return;
                }

                if (!is_running()) return;

                log("Acknowledged destruction of room `" << rooms[port]->name << "`.");
                
                std::unique_lock<std::mutex> lock(delete_mutex);
                rooms_to_delete.push_back(port);

            }, [&](int port, GameServiceProvider* gsp){
                games[port].reset(gsp);
            }, [&](int port){
                if (games.count(port) == 0){
                    log("CRITICAL ERROR! Ghost game provider detected!");
                    return;
                }

                if (!is_running()) return;

                log("Acknowledged destruction of game `" << games[port]->name << "`.");
                
                std::unique_lock<std::mutex> lock(delete_mutex);
                games_to_delete.push_back(port);
            }, name);
            
            int port = rsp->address.port;
            rsp->start_async();
            rooms[port].reset(rsp);

            RoomDetail rd; {
                strncpy(rd.name, name, sizeof(RoomName));
                rd.port = port;
            }
            send_message(Server::CREATE_OK, &rd, sizeof(RoomDetail), peer);

            auto room_details = get_room_details();
            broadcast_message(Server::ROOM_LIST, room_details.data(), sizeof(RoomDetail) * room_details.size());

            return;
        };
    }
}

#undef log