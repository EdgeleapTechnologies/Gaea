#include "String.h"

#include <Core/Memory.h>
#include <Core/Types.h>

namespace Edgeleap
{

    String::String(const char* string)
    {
        this->m_string_length = String::ComputeStringLength(string);
        this->m_string = (char*)GlobalAllocate(this->m_string_length);
        MemCopy((void*)this->m_string.ptr, (void*)string, this->m_string_length);
    }

    String::String(const char* string, size_t length)
    {
        this->m_string_length = length;
        this->m_string = (char*)GlobalAllocate(this->m_string_length);
        MemCopy((void*)this->m_string.ptr, (void*)string, this->m_string_length);
    }

    //TODO(Tiago):very naive strlen implementation, in the future improve it
    //using SIMD
    size_t String::ComputeStringLength(const char* string)
    {
        size_t counter = 0;
        while(string[counter] != 0)
        {
            counter++;
        }
        return counter;
    }

}
