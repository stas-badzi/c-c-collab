#pragma once

#if defined _WIN32
    #if defined __cplusplus
        #define libexport extern "C" __stdcall __declspec(dllexport)
        #define libimport extern "C" __stdcall __declspec(dllimport)
    #else
        #define libexport __stdcall __declspec(dllexport)
        #define libimport _stdcall __declspec(dllimport)
    #endif
#else
    #if defined __cplusplus
        #define libimport extern "C"
        #if defined LIBRARY_EXPORT
           #define libexport extern "C" __attribute__((visibility("default")))
        #else
           #define libexport extern "C"
        #endif
    #else
        #define libimport 
        #if defined LIBRARY_EXPORT
           #define libexport __attribute__((visibility("default")))
        #else
           #define libexport 
        #endif
    #endif
    
#endif













/*
#ifndef _DYNLIB
#define _DYNLIB

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
   //define something for Windows (32-bit and 64-bit, this part is common)
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #else
      //define something for Windows (32-bit only)
   #endif
#elif __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
         // iOS, tvOS, or watchOS Simulator
    #elif TARGET_OS_MACCATALYST
         // Mac's Catalyst (ports iOS API into Mac, like UIKit).
    #elif TARGET_OS_IPHONE
        // iOS, tvOS, or watchOS device
    #elif TARGET_OS_MAC
        // Other kinds of Apple platforms
    #else
        #error "Unknown Apple platform"
    #endif
#elif __ANDROID__
    // Below __linux__ check should be enough to handle Android,
    // but something may be unique to Android.
#elif __linux__
    // linux
    #define libexport extern "C" //_declspec(dllexport)
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
    #error "Unknown compiler"
#endif


#endif
*/