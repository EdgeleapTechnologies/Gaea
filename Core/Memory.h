#pragma once

#include <Core/Types.h>

namespace Edgeleap
{

void* GlobalAllocate(size_t size);
void GlobalFree(void* ptr);
void MemCopy(void* destination, void* source, size_t size);

}
