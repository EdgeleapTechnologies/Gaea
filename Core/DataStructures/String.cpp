#include "String.h"

#include <Core/Memory.h>
#include <Core/Types.h>

namespace Edgeleap
{

//TODO(Tiago):very naive strlen implementation, in the future improve it
//using SIMD
size_t ComputeStringLength(const char* string)
{
    size_t counter = 0;
    while(string[counter] != 0)
    {
        counter++;
    }
    return counter;
}

String::String(const char* string, Allocator* allocator)
{
    this->m_string_length = ComputeStringLength(string);
    this->m_string = {(char*)allocator->Allocate(this->m_string_length), allocator};
    MemCopy((void*)this->m_string.ptr, (void*)string, this->m_string_length);
}

String::String(const char* string, size_t length, Allocator* allocator)
{
    this->m_string_length = length;
    this->m_string = {(char*)allocator->Allocate(this->m_string_length), allocator};
    MemCopy((void*)this->m_string.ptr, (void*)string, this->m_string_length);
}

String::String(const String& other)
{
    if(&other == this) return;

    Allocator* allocator = other.m_string.allocator;
    this->m_string_length = other.m_string_length;
    this->m_string = {
        (char*)allocator->Allocate(this->m_string_length),
        allocator
    };
    MemCopy((void*)this->m_string.ptr, (void*)other.m_string.ptr, this->m_string_length);
}

String& String::operator=(const String& other)
{
    if(&other == this) return *this;

    Allocator* allocator = other.m_string.allocator;
    this->m_string_length = other.m_string_length;
    this->m_string = {
        (char*)allocator->Allocate(this->m_string_length),
        allocator
    };
    MemCopy((void*)this->m_string.ptr, (void*)other.m_string.ptr, this->m_string_length);

    return *this;
}

String::String(String&& other)
{
    if(&other == this) return;

    this->m_string_length = other.m_string_length;
    other.m_string_length = 0;

    this->m_string = move(other.m_string);
}

String& String::operator=(String&& other)
{
    if(&other == this) return *this;

    this->m_string_length = other.m_string_length;
    other.m_string_length = 0;

    this->m_string = move(other.m_string);
    return *this;
}

char& String::operator[](const size_t index)
{
    return this->m_string.ptr[index];
}

char String::at(const size_t index) const
{
    return this->m_string.ptr[index];
}

size_t String::legth() const
{
    return this->m_string_length;
}

const char* String::data() const
{
    return this->m_string.ptr;
}


}
