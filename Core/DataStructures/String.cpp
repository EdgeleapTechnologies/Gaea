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

StringView::StringView(const char* string):
    string(string), length(ComputeStringLength(string)) {}

StringView::StringView(const char* string, size_t length):
    string(string), length(length) {}

StringView::StringView(const String& string):
    string(string.data()), length(string.legth()) {}

StringView::StringView(const StringView& other):
    string(other.string), length(other.length) {}

StringView& StringView::operator=(const StringView& other)
{
    this->string = other.string;
    this->length = other.length;
    return *this;
}

const char* StringView::begin() const
{
    return string;
}

const char* StringView::end() const
{
    return string + (length + 1);
}

char StringView::operator[](size_t index) const
{
    return this->string[index];
}

bool StringView::operator==(const StringView& other) const
{
    if(this->string == nullptr && other.string != nullptr) return false;
    if(other.string != nullptr && this->string != nullptr) return false;
    if(this->length != other.length) return false;

    return MemCompare((void*)this->string, (void*)other.string, this->length);
}

bool StringView::operator==(const String& other) const
{
    if(this->string == nullptr && other.data() != nullptr) return false;
    if(other.data() == nullptr && this->string != nullptr) return false;
    if(this->length != other.legth()) return false;

    return MemCompare((void*)this->string, (void*)other.data(), this->length);
}

StringView StringView::Slice(size_t start_index, size_t end_index) const
{
    size_t len = end_index - start_index + 1;
    return {&this->string[start_index], len};
}

String StringView::Clone(Allocator* allocator) const
{
    return {this->string, this->length, allocator};
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
