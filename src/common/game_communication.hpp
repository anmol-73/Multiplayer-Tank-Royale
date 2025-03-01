#ifndef H_COM_GAME_COMMUNICATION
#define H_COM_GAME_COMMUNICATION

#include "game.hpp"

namespace Game
{
    Game::GameState * deserialize_game_state(const void *, size_t sz);

    std::pair<void *, size_t> serialize_game_state(Game::GameState game_state);
}

#endif