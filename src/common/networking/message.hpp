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

        namespace Lobby
        {
            enum Client{ // The type of messages a client would send while in the lobby
                NAME_SET_REQUEST,
                START_GAME_REQUEST,
                REMOVE_PLAYER_REQUEST
            };

            enum Server{ // The type of messages the server can send while in the lobby
                NAME_SET,
                GAME_START,
                DISCONNECT
            };
        } // namespace Lobby

        namespace Game
        {
            
        } // namespace Game
        
    } // namespace Message
    
} // namespace Networking


#endif