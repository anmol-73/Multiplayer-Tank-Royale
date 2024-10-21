#ifndef HEADER_VOID_PTR
#define HEADER_VOID_PTR

#include <cstdlib>
#include <memory>

struct free_delete
{
    void operator()(void* x);
};

using unique_void_ptr = std::unique_ptr<void, free_delete>;

#endif