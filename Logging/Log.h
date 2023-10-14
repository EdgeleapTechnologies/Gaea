#pragma once

#include <iostream>

namespace Edgeleap
{

#define SUCCESS_LOG(s, ...) printf(s __VA_OPT__(, ) __VA_ARGS__)

}
