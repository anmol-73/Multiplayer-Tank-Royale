#include "game_communication.hpp"

Game::GameState *Game::deserialize_game_state(const void *, size_t sz)
{
    return nullptr;
}

template<typename T>
void* store(void* ptr, const T& item){
    *reinterpret_cast<T*>(ptr) = item;
    return reinterpret_cast<T*>(ptr) + 1;
}

std::pair<void *, size_t> Game::serialize_game_state(Game::GameState game_state)
{
    void * message;
    size_t sz = 0;

    sz += 2*sizeof(size_t);
    sz += sizeof(long double);
    sz += sizeof(std::chrono::time_point<std::chrono::high_resolution_clock>);

    sz += 12*sizeof(Game::GameState::PlayerState);

    size_t projectiles_num = game_state.projectile_vector.size();
    sz += sizeof(size_t);
    sz += projectiles_num*sizeof(Game::GameState::Projectile);

    size_t explosions_num = game_state.explosion_vector.size();
    sz += sizeof(size_t);
    sz += explosions_num*sizeof(Game::GameState::Explosion);

    sz += sizeof(Game::Frame);

    message = malloc(sz);
    void* ptr = message;

    // sz += 2*sizeof(size_t);
    ptr = store(ptr, game_state.num_of_players);
    ptr = store(ptr, game_state.map_num);
    
    // sz += sizeof(long double);
    ptr = store(ptr, game_state.time_of_last_objects_update);

    // sz += sizeof(std::chrono::time_point<std::chrono::high_resolution_clock>);
    ptr = store(ptr, game_state.start_time);

    // sz += 12*sizeof(Game::GameState::PlayerState);
    for(size_t i=0; i<12; i++)
    {
        ptr = store(ptr, game_state.player_vector[i]);
    }

    // size_t projectiles_num = game_state.projectile_vector.size();
    // sz += sizeof(size_t);
    ptr = store(ptr, projectiles_num);

    // sz += projectiles_num*sizeof(Game::GameState::Projectile);
    for(size_t i=0; i<projectiles_num; i++)
    {
        ptr = store(ptr, game_state.projectile_vector[i]);
    }

    // size_t explosions_num = game_state.explosion_vector.size();
    // sz += sizeof(size_t);
    ptr = store(ptr, explosions_num);

    // sz += explosions_num*sizeof(Game::GameState::Explosion);
    for(size_t i=0; i<explosions_num; i++)
    {
        ptr = store(ptr, game_state.explosion_vector[i]);
    }

    // sz += sizeof(Game::Frame);
    ptr = store(ptr, game_state.curr_frame);

    return {message, sz};
}
