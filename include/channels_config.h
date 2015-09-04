#ifndef CHANNELS_CONFIG_H_INCLUDED
#define CHANNELS_CONFIG_H_INCLUDED4

#if __cplusplus < 201103 // Check CPP 03
#	error "This library needs at least CPP 03 standard compiller"
#endif  // Check CPP 03

#include <boost/config.hpp>
#include <boost/move/move.hpp>


#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	define MSVC_CL
/*	Disable warnings:
  4251 - class need DLL-interface will be used for each public API class
  4710 - function not inlined
  4711 - function selected for automatic inline expansion
*/
//#	pragma warning (disable : 4251)
#	pragma warning (disable : 4710)
#	pragma warning (disable : 4711)
#	pragma once
#endif


#if defined(_WIN32) || defined(_WIN64)
#	ifndef PLATFORM_WINDOWS
#		define PLATFORM_WINDOWS 1
#	endif
#endif

#if defined(__unix__) || defined(__linux__)
#	ifndef PLATFORM_UNIX
#		define PLATFORM_UNIX 1
#	endif
#	include <unistd.h>
#endif

// Shared lib / DLL special keywords if any
#ifdef PLATFORM_WINDOWS
#	include "win/winver.h"
#	include <windows.h>

// MSVC and MinGW Windows DLL
#	ifdef CHANNEL_BUILD_DLL
#		define CHANNEL_PUBLIC __declspec(dllexport)
#	elif defined(CHANNEL_STATIC_LIB)
#		define CHANNEL_PUBLIC
#	else
#		define CHANNEL_PUBLIC __declspec(dllimport)
#	endif

#else
// UNIX(ELF32/64)
#	define CHANNEL_PUBLIC
#endif

// Whether exception handling is disabled.
#ifndef CHANNELS_NO_EXCEPTIONS
# ifdef BOOST_NO_EXCEPTIONS
#  define CHANNELS_NO_EXCEPTIONS 1
# endif // BOOST_NO_EXCEPTIONS)
#endif // CHANNELS_NO_EXCEPTIONS

#include <stdint.h>


#endif // CHANNELS_CONFIG_H_INCLUDED
