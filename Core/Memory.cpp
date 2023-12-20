#include "Memory.h"
#include "Core/Types.h"

#include <stdlib.h>

namespace Edgeleap
{

void* GlobalAllocate(size_t size)
{
    return ::malloc(size);
}

void GlobalFree(void* ptr)
{
    ::free(ptr);
}

//TODO(Tiago):naive implementation of memcpy, improve it using
//things like SIMD when you can
void MemCopy(void* destination, void* source, size_t size)
{
    for(size_t i = 0; i < size; ++i)
    {
        ((uint8*)destination)[i] = ((uint8*)source)[i];
    }
}

}
