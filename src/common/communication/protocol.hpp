#ifndef H_COM_PROTOCOL
#define H_COM_PROTOCOL

namespace Communication
{
    namespace Lobby
    {
        using RoomName = char[32];

        struct RoomDetail{
            RoomName name;
            int port;
        };
        
        // The commands sent by the lobby server
        enum Server{
            ROOM_LIST, // data: `RoomDetail[]`. The list of all active rooms.
            CREATE_DENIED, // No data. Room could not be created due to name conflict.
            CREATE_OK, // data: `RoomDetail`. The room just created..
        };

        // The commands sent by the lobby client
        enum Client{
            CREATE_ROOM, // data: `RoomName`. The name of the new room.
        };
    } // namespace Lobby   

    namespace Room
    {
        using PlayerName = char[32];
        struct PlayerDetail{
            PlayerName name;
            int id; // (Yes it is not required to send the id in RESIDENT_LIST, but it could be useful in the future)
        }; 
        
        struct RoomSettings{
            int map; // The index of the map used.
        };

        // TODO: Think of a nice way for room to be joinable while game is running!
        // The commands sent by the room server
        enum Server{
            JOIN_DENIED, // No data. Client is not allowed to join since room is full.
            JOIN_OK, // data: `int`. The ID assigned to the player
            
            PLAYER_LIST, // data: `PlayerDetail[]`. The list of all players in the room.
            
            NAME_DENIED, // No data. Name alreay in use.
            NAME_OK, // No data. Name set request accepted.

            SETTINGS_SET, // data: `RoomSettings`.
            
            JOIN_GAME, // data: `int`. The adress of the game server to connect to.
        };

        // The commands sent by the room client
        enum Client{
            SET_NAME, // data: `PlayerName`. The name to be used.
            
            SET_SETTINGS, // data: `RoomSettings`.
            START_GAME, // No data. The game is to be started now.
        };
    } // namespace Room
    
    namespace Game
    {

        using PlayerIdentification = ::Communication::Room::PlayerDetail;
        // The commands sent by the game server
        enum Server{
            RESPAWN_OK,
            GAME_OVER,
            GAME_STATE_BROADCAST,
        };

        // The commands sent by the game client
        enum Client{
            IDENTIFY,
            TYPES_SELECTION,
            FRAME,
        };
    } // namespace Game
    
} // namespace Communication

#endif