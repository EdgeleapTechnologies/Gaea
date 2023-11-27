#pragma once

#ifdef __linux__
#define PLATFORM_LINUX 1
#endif

#ifdef _WIN32
#define PLATFORM_WINDOWS 1
#include <Windows.h>
#endif
