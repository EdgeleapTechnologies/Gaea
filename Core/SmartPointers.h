#pragma once

#include <Core/Memory.h>

namespace Edgeleap
{

template<typename T, typename del = decltype(&GlobalFree)>
class AutoPtr
{
public:
    T* ptr = nullptr;

public:
    AutoPtr() = default;

    AutoPtr(const AutoPtr&) = delete;
    AutoPtr& operator=(const AutoPtr&) = delete;

    AutoPtr(T* const ptr): ptr(ptr){};
    AutoPtr(decltype(nullptr)): ptr(nullptr){};

    AutoPtr(T*&& ptr)
    {
        this->ptr = ptr;
        ptr = nullptr;
    }

    AutoPtr(AutoPtr&& other)
    {
        this->ptr = other.ptr;
        other.ptr = nullptr;
    }

    ~AutoPtr()
    {
        if(this->ptr == nullptr) return;
        (void)del((void*)this->ptr);
    }

    AutoPtr& operator=(AutoPtr&& other)
    {
        if(this == &other) return *this;

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
