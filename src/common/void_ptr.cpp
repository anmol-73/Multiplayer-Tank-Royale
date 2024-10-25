#include "void_ptr.hpp"

void free_delete::operator()(void *x) { free(x); }
