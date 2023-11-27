#pragma once

#include <Core/SmartPointers.h>

namespace Edgeleap
{

class String
{
private:
    AutoPtr<char> m_string = nullptr;
};

}
