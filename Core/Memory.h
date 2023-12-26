#pragma once

#include <Core/Types.h>

namespace Edgeleap
{

void* GlobalAllocate(size_t size);
void GlobalFree(void* ptr);
void MemCopy(void* destination, void* source, size_t size);
bool MemCompare(void* buffer1, void* buffer2, size_t length);
void MemSet(void* destination, size_t length, u8 ValueToSet);
inline void MemZero(void* destination, size_t length)
{
    MemSet(destination, length, 0);
}

// Allocator interface that can be used as generic allocator input
// for other methods
class Allocator
{
protected:
    Allocator() = default;

public:
    virtual ~Allocator() = default;
    Allocator(const Allocator& other) = delete;
    Allocator(Allocator&& other) = delete;
    Allocator& operator=(const Allocator& other) = delete;
    Allocator& operator=(Allocator&& other) = delete;

    virtual void* Allocate(size_t length) = 0;
    virtual void Free(void* ptr) = 0;
};

class SystemAllocator: public Allocator
{
public:
    SystemAllocator() = default;

    static SystemAllocator& Instance();

    inline void* Allocate(size_t length) override
    {
        return GlobalAllocate(length);
    }

    inline void Free(void* ptr) override
    {
        GlobalFree(ptr);
    }
};

}
