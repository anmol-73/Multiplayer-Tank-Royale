#ifndef HEADER_COMMON_NETWORKING_MESSAGE
#define HEADER_COMMON_NETWORKING_MESSAGE
#include <raylib.h>
#include <stdlib.h>

namespace Networking
{
    namespace Message
    {
        // For the future where we can have multiple lobbies
        // namespace Lobby
        // {
        //     enum Server{
        //         ROOM_ADDRESS_SET
        //     };
        //     typedef char RoomAddress[50]; // https://superuser.com/questions/381022/how-many-characters-can-an-ip-address-be
        // } // namespace Lobby
        

        namespace Room
        {
            const size_t STRING_MESSAGE_SIZE = 64;
            const size_t MAX_ROOM_SIZE = 12;
            const size_t NAME_SIZE = 32;
            enum Client{ // The type of messages a client would send while in the room
                NAME_SET_REQUEST,
                START_GAME_REQUEST,
                REMOVE_PLAYER_REQUEST,
                MAP_SET_REQUEST,
                GAME_STATE_UPDATE_REQUEST,
                SECONDARY_STATE_UPDATE_REQUEST,
                REQUEST_SPAWN_DATA
            };

            struct NameSetRequest{
                size_t client_id;
                char name[NAME_SIZE];
            };
            
            typedef NameSetRequest LobbyNameBroadcast[MAX_ROOM_SIZE];

            struct SpawnData{
                size_t map_id;
                Vector2 spawn;
                float angle;
            };

            enum Server{ // The type of messages the server can send while in the room
                CONNECT_OK,
                CONNECT_DENIED,
                ROOM_LIST_BROADCAST,
                GAME_START,
                DISCONNECT,
                MAP_SET,
                GAME_STATE_UPDATE,
                SECONDARY_STATE_UPDATE,
                SET_SPAWN_DATA
            };
        } // namespace Room

        namespace Game
        {
            
        } // namespace Game
        
    } // namespace Message
    
} // namespace Networking


#endif