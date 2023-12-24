#pragma once

#include <Core/SmartPointers.h>
#include <Core/Types.h>

namespace Edgeleap
{

class String
{
private:
    AutoPtr<char> m_string = nullptr;
    size_t m_string_length = 0;

public:
    String() = default;
    ~String() = default;

    String(const char* string);
    String(const char* string, size_t length);

    //rule of 5 operators & constructors
    String(const String& other);
    String(String&& other);
    String& operator=(const String& other);
    String& operator=(String&& other);

    char& operator[](size_t index);
    char at(size_t index) const;
    size_t legth() const;
    const char* data() const;

    static size_t ComputeStringLength(const char* string);
};

}
