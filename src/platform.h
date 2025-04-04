#pragma once

#ifndef PLATFORM_H
#define PLATFORM_H

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
    #define PLATFORM_NAME "Windows"
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #define PLATFORM_IOS
        #define PLATFORM_NAME "iOS"
    #elif TARGET_OS_MAC
        #define PLATFORM_MACOS
        #define PLATFORM_NAME "macOS"
    #endif
#elif defined(__ANDROID__)
    #define PLATFORM_ANDROID
    #define PLATFORM_NAME "Android"
#elif defined(__linux__)
    #define PLATFORM_LINUX
    #define PLATFORM_NAME "Linux"
#else
    #error "Unknown platform"
#endif

// Architecture detection
#if defined(__arm__) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64)
    #if defined(__aarch64__) || defined(_M_ARM64)
        #define PLATFORM_ARCH_ARM64
        #define PLATFORM_ARCH "arm64"
    #else
        #define PLATFORM_ARCH_ARM
        #define PLATFORM_ARCH "arm"
    #endif
#elif defined(__x86_64__) || defined(_M_X64)
    #define PLATFORM_ARCH_X64
    #define PLATFORM_ARCH "x64"
#elif defined(__i386__) || defined(_M_IX86)
    #define PLATFORM_ARCH_X86
    #define PLATFORM_ARCH "x86"
#else
    #define PLATFORM_ARCH_UNKNOWN
    #define PLATFORM_ARCH "unknown"
#endif

#endif
