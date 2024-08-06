// DO NOT include this file into the XMLFoundation library
// ONLY use it with the Kotlin Compiler


//////////////////////////////////////////////////////////////
// prepare the XMLFoundation preprocessor compilation directives
// these are the platform related preprocessor directives within the XMLFundation codebase
//
//_WIN32	// identifies all Windows platforms 
//_WIN64        // target is 64 bit Windows 
//___LINUX	// IDentifies all Linux builds [32, 64 and various linux types] -  (including Android because Android originates from a fork of Linux and shares much of its structure)
//___LINUX64    // added in addition to ___LINUX when the target is 64 bit
//___IOS	// All Apple platforms are considered 64 bit within XMLFoundation
//___ARM32	// specifically only defined for 32 bit ARMv7 
//___ARM64	// All 64bit 
//___ANDROID	// Specifies the reduced branch of the Linux build, currently must Accompany either ___ARM32 or ___ARM64
//___KOTLIN_COMPILER
// _CLANG_COMPILER_


//
//
// BEGIN - convert the CMake/Kotlin supplied preprocessors into the XMLFoundation directives mentioned above
//
//


// Start with all values undefined
#ifdef _WIN32
#undef _WIN32
#endif
#ifdef _WIN64
#undef _WIN64
#endif
#ifdef ___LINUX
#undef ___LINUX
#endif
#ifdef ___LINUX64
#undef ___LINUX64
#endif
#ifdef ___IOS
#undef ___IOS
#endif
#ifdef ___ARM32
#undef ___ARM32
#endif
#ifdef ___ARM64
#undef ___ARM64
#endif
#ifdef ___ANDROID
#undef ___ANDROID
#endif



#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
#if defined(__ARM_PCS_VFP)
//		#define ABI "armeabi-v7a/NEON (hard-float)"
#define ___ARM32
#else
#define ABI "armeabi-v7a/NEON"
#define ___ARM32
#endif
#else
#if defined(__ARM_PCS_VFP)
#define ABI "armeabi-v7a (hard-float)"
#define ___ARM32
#else
#define ABI "armeabi-v7a"
#define ___ARM32
#endif
#endif
#else
#define ABI "armeabi"
#define ___ARM32
#endif
#elif defined(__i386__)
#define ABI "x86"
// this is intentionally designed to fail here during the build of [x86]
build-error("----ONLY [armeabi-v7a] and [arm64-v8a] targets are currently supported in Android Studio not [x86]")

#elif defined(__x86_64__)
#define ABI "x86_64"
// this is intentionally designed to fail here during the build of [x86_64]
build-error("----ONLY [armeabi-v7a] and [arm64-v8a] targets are currently supported in Android Studio not [x86_64])

#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
#define ABI "mips64"
#define ___MIPS64 1
#elif defined(__mips__)
#define ABI "mips"
#define ___MIPS 1
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#define ___ARM64 1
#else
#define ABI "unknown"
#endif

#if defined(___ARM32) || defined(___ARM64)
#define ___LINUX 
#define ___ANDROID
#endif





#if defined(_WIN64)
#ifndef _WIN32
#define _WIN32
#endif
#endif
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define RPC_NO_WINDOWS_H
//  #define _KRPCENV_
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif
#define __int64 long long
#define _KOTLIN_A_STUDIO

//  #define _CRT_BUILD_DESKTOP_APP 0
//  #define __clang__
#endif


/*
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define _WINSOCKAPI_
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS						// this is being addressed in 2024
	#endif
	#define _WINSOCK_DEPRECATED_NO_WARNINGS					// TODO: change inet_ntoa() to inet_ntop() then remove the _WINSOCK_DEPRECATED_NO_WARNINGS #define
	#include <wtypes.h>
	#include <winsock2.h>
	#include <windows.h>
#endif
*/




