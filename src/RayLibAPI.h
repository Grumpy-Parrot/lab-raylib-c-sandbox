#pragma once

#ifndef RAYLIB_API_H
#define RAYLIB_API_H

#include "Platform.h"

#if defined(PLATFORM_MACOS)
    #include "../external/raylib-5.5/macOS-universal/include/raylib.h"
    #include "../external/raylib-5.5/macOS-universal/include/raymath.h"
    #include "../external/raylib-5.5/macOS-universal/include/rlgl.h"
#elif defined(PLATFORM_WINDOWS)
    #include "../external/raylib-5.5/Windows-x64/include/raylib.h"
    #include "../external/raylib-5.5/Windows-x64/include/raymath.h"
    #include "../external/raylib-5.5/Windows-x64/include/rlgl.h"
#elif defined(PLATFORM_LINUX)
    #include "../external/raylib-5.5/Linux-x64/include/raylib.h"
    #include "../external/raylib-5.5/Linux-x64/include/raymath.h"
    #include "../external/raylib-5.5/Linux-x64/include/rlgl.h"
#elif defined(PLATFORM_WASM)
    #include "../external/raylib-5.5/Wasm/include/raylib.h"
    #include "../external/raylib-5.5/Wasm/include/raymath.h"
    #include "../external/raylib-5.5/Wasm/include/rlgl.h"
#endif

#endif
