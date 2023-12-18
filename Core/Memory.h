#pragma once

#include <Core/Types.h>
#include <string.h>

namespace Edgeleap
{

void* GlobalAllocate(size_t size);
void GlobalFree(void* ptr);

}
