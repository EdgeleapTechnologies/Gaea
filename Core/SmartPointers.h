#pragma once

#include <Core/Memory.h>

namespace Edgeleap
{

template<typename T>
class AutoPtr
{
public:
    Allocator* allocator = nullptr;
    T* ptr = nullptr;

public:
    AutoPtr() = default;

    AutoPtr(const AutoPtr&) = delete;
    AutoPtr& operator=(const AutoPtr&) = delete;

    AutoPtr(const T* const ptr, Allocator* allocator = &SystemAllocator::Instance()): ptr(ptr), allocator(allocator){};
    AutoPtr(decltype(nullptr)): ptr(nullptr){};

    ~AutoPtr()
    {
        if(this->ptr == nullptr) return;

        this->ptr->~T();

        if(this->allocator == nullptr) return;
        this->allocator->Free((void*)this->ptr);
    }

    AutoPtr(T*&& ptr, Allocator* allocator = &SystemAllocator::Instance()): allocator(allocator)
    {
        this->ptr = ptr;
        ptr = nullptr;
    }

    AutoPtr(AutoPtr&& other)
    {
        if(&other == this) return;
        this->~AutoPtr();

        this->allocator = other.allocator;
        other.allocator = nullptr;

        this->ptr = other.ptr;
        other.ptr = nullptr;
    }

    AutoPtr& operator=(AutoPtr&& other)
    {
        if(this == &other) return *this;
        this->~AutoPtr();

        this->allocator = other.allocator;
        other.allocator = nullptr;

        this->ptr = other.ptr;
        other.ptr = nullptr;

        return *this;
    }

    T& operator*()
    {
        return *this->ptr;
    }

    T* operator->()
    {
        return this->ptr;
    }

    operator bool()
    {
        return this->ptr != nullptr;
    }
};

}
