#ifndef H_COM_GAME_COMMUNICATION
#define H_COM_GAME_COMMUNICATION

#include "game.hpp"
#include "misc/void_ptr.hpp"

namespace Game
{
    Game::GameState deserialize_game_state(const void * message, size_t sz);

    std::pair<Utils::unique_void_ptr, size_t> serialize_game_state(Game::GameState game_state);
}

#endif