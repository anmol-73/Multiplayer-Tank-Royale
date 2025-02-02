#include "void_ptr.hpp"

void Utils::free_delete::operator()(void *x)
{
    free(x);
}