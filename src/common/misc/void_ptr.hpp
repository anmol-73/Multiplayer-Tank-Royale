#ifndef H_COM_VOID_PTR
#define H_COM_VOID_PTR

#include <cstdlib>
#include <memory>

namespace Utils
{
    struct free_delete
    {
        void operator()(void *x);
    };
    using unique_void_ptr = std::unique_ptr<void, free_delete>;
} // namespace Utils


#endif