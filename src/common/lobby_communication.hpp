namespace Networking
{
    enum ResponseStatus{
        CONNECT_OK,
        CONNECT_DENIED
    };
    namespace Lobby
    {
        enum ResponseType{
            JOIN_RESPONSE,
            NEW_PLAYER
        };

        enum RequestType{
            SET_NAME,
            START_GAME
        };

        struct ClientSetName{
            char client_name[128];
        };

        struct JoinResponse{
            ResponseStatus status;
            size_t client_id;
            char message[64]; // NOTE: Increase the message size if required!
        };
    } // namespace Lobby
} // namespace Networking
