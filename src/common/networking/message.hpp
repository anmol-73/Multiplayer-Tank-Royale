#ifndef HEADER_COMMON_NETWORKING_MESSAGE
#define HEADER_COMMON_NETWORKING_MESSAGE

namespace Networking
{
    namespace Message
    {
        enum ConnectStatus{
            OK,
            DENIED
        };

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
            const size_t MAX_ROOM_SIZE = 8;
            enum Client{ // The type of messages a client would send while in the room
                NAME_SET_REQUEST,
                START_GAME_REQUEST,
                REMOVE_PLAYER_REQUEST
            };

            typedef char Name[32];
            typedef struct RoomMember{
                size_t client_id;
                Name name;
            } RoomMemberBroadcast[MAX_ROOM_SIZE];

            enum Server{ // The type of messages the server can send while in the room
                ASSIGN_ID,
                ROOM_LIST_BROADCAST,
                GAME_START,
                DISCONNECT
            };
        } // namespace Room

        namespace Game
        {
            
        } // namespace Game
        
    } // namespace Message
    
} // namespace Networking


#endif