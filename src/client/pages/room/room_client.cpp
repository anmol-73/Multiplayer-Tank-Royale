#include "room_client.hpp"

bool ServiceConsumers::RoomClient::is_connected()
{
    return _is_connected;
}

void ServiceConsumers::RoomClient::handle_message(Communication::Command type, const void *message, size_t size)
{
}

void ServiceConsumers::RoomClient::on_start()
{
}

void ServiceConsumers::RoomClient::on_finish()
{
}

std::string ServiceConsumers::RoomClient::on_connection_established()
{
    return std::string();
}

void ServiceConsumers::RoomClient::handle_disconnection()
{
}
