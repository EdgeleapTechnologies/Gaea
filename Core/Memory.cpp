#include "Memory.h"
#include "Core/Types.h"

#include <stdlib.h>

namespace Edgeleap
{

SystemAllocator& SystemAllocator::Instance()
{
    static SystemAllocator instance;
    return instance;
}

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

bool MemCompare(void* buffer1, void* buffer2, size_t length)
{
    u8* a = (u8*)buffer1;
    u8* b = (u8*)buffer2;
    for(size_t i = 0; i < length; ++i)
    {
        if(a[i] != b[i]) return false;
    }
    return true;
}

void MemSet(void* destination, size_t length, u8 ValueToSet)
{
    u8* buffer = (u8*)destination;
    for(size_t i = 0; i < length; ++i)
    {
        buffer[i] = ValueToSet;
    }
}

}
