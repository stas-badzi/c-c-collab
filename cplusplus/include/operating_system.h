#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
   //define something for Windows (32-bit and 64-bit, this part is common)
   #define OS "Windows"
   #ifdef _WIN64
        #define OS_ "Windows x64"
      //define something for Windows (64-bit only)
   #else
        #define OS_ "Windows x32"
      //define something for Windows (32-bit only)
   #endif
#elif __APPLE__
    #define OS "Apple"
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
        #define OS_ "Apple Iphone Symulator"
         // iOS, tvOS, or watchOS Simulator
    #elif TARGET_OS_MACCATALYST
        #define OS_ "Apple Maccatalyst"
         // Mac's Catalyst (ports iOS API into Mac, like UIKit).
    #elif TARGET_OS_IPHONE
        #define OS_ "Apple Iphone"
        // iOS, tvOS, or watchOS device
    #elif TARGET_OS_MAC
        #define OS_ "Apple Other"
        // Other kinds of Apple platforms
    #else
        #define OS_ "Apple Unknown"
        #error "Unknown Apple platform"
    #endif
#elif __ANDROID__
    #define OS "Android"
    #define OS_ "Android"
    // Below __linux__ check should be enough to handle Android,
    // but something may be unique to Android.
#elif  __FreeBSD__
    #define OS "FreeBSD"
    #define OS_ "FreeBSD"
    // freebsd
#elif __linux__
    #define OS "Linux"
    #define OS_ "Linux"
    // linux
#elif __unix__ // all unices not caught above
    #define OS "Unix"
    #define OS_ "Unix"
    // Unix
#elif defined(_POSIX_VERSION)
    #define OS "Posix"
    #define OS_ "Posix " + _POSIX_VERSION
    // POSIX
#else
    #define OS "Unknown"
    #define OS_ "Unknown"
    #error "Unknown compiler"
#endif