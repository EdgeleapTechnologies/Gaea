#pragma once

#include <Core/Memory.h>
#include <Core/SmartPointers.h>
#include <Core/Types.h>

namespace Edgeleap
{

size_t ComputeStringLength(const char* string);

struct String;

struct StringView
{
    char* string = nullptr;
    size_t length = 0;

    StringView(const char* string);
    StringView(const char* string, size_t length);
    StringView(const String& string);

    StringView() = default;
    ~StringView() = default;
    StringView(const StringView& other);
    StringView(StringView&& other);
    StringView& operator=(const StringView& other);
    StringView& operator=(StringView&& other);

    char* begin() const;
    char* end() const;

    char operator[](size_t index) const;
    StringView Slice(size_t start_index, size_t end_index);
    String Clone() const;
};

struct String
{
private:
    AutoPtr<char> m_string = nullptr;
    size_t m_string_length = 0;

public:
    String() = default;
    ~String() = default;

    String(const char* string, Allocator* allocator = &SystemAllocator::Instance());
    String(const char* string, size_t length, Allocator* allocator = &SystemAllocator::Instance());

    //rule of 5 operators & constructors
    String(const String& other);
    String(String&& other);
    String& operator=(const String& other);
    String& operator=(String&& other);

    char& operator[](size_t index);
    char at(size_t index) const;
    size_t legth() const;
    const char* data() const;
};

}
