#include "Memory.h"

#include <stdlib.h>

namespace Edgeleap
{

void* GlobalAllocate(size_t size)
{
    return malloc(size);
}

}
