#include "game_communication.hpp"

template<typename T>
const void* destore(const void* ptr, T& item) {
    item = *reinterpret_cast<const T*>(ptr); 
    return reinterpret_cast<const T*>(ptr) + 1; 
}

template<typename T>
const void* destore_list(const void* ptr, std::list<T>& vec)
{
    size_t size;
    ptr = destore(ptr, size);  // Extract vector size

    
    for (size_t i = 0; i < size; i++) {
        T item;
        ptr = destore(ptr, item);
        vec.push_back(item);
    }
    return ptr;
}



Game::GameState Game::deserialize_game_state(const void *message, size_t sz)
{
    Game::GameState game_state;
    const void* ptr = message;
    ptr = destore(ptr, game_state.num_of_players);
    ptr = destore(ptr, game_state.map_num);
    ptr = destore(ptr, game_state.time_of_last_objects_update);
    ptr = destore(ptr, game_state.start_time);
    game_state.player_vector.resize(12);
    for (size_t i = 0; i < 12; i++) {
        ptr = destore(ptr, game_state.player_vector[i]);
    }
    ptr = destore_list(ptr, game_state.projectile_vector);
    ptr = destore_list(ptr, game_state.explosion_vector);
    ptr = destore(ptr, game_state.curr_frame);
    return game_state;


    
}

template<typename T>
void* store(void* ptr, const T& item){
    *reinterpret_cast<T*>(ptr) = item;
    return reinterpret_cast<T*>(ptr) + 1;
}

std::pair<Utils::unique_void_ptr, size_t> Game::serialize_game_state(Game::GameState game_state)
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
    for(auto it = game_state.projectile_vector.begin(); it != game_state.projectile_vector.end(); ++it)
    {
        ptr = store(ptr, *it);
    }

    // size_t explosions_num = game_state.explosion_vector.size();
    // sz += sizeof(size_t);
    ptr = store(ptr, explosions_num);

    // sz += explosions_num*sizeof(Game::GameState::Explosion);
    for(auto it = game_state.explosion_vector.begin(); it != game_state.explosion_vector.end(); ++it)
    {
        ptr = store(ptr, *it);
    }

    // sz += sizeof(Game::Frame);
    ptr = store(ptr, game_state.curr_frame);

    return {Utils::unique_void_ptr(message), sz};
}