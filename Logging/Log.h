#pragma once

#include <iostream>

namespace Edgeleap
{

#define ERROR_LOG(X, ...) printf(X, __VA_ARGS__);
#define WARNING_LOG(X, ...) printf(X, __VA_ARGS__);
#define SUCCESS_LOG(X, ...) printf(X, ##__VA_ARGS__);

}
