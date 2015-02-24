#ifndef CHANNELS_CONFIG_H_INCLUDED
#define CHANNELS_CONFIG_H_INCLUDED4

#include <boost/config.hpp>

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

#if defined(__unix__)
#	define PLATFORM_UNIX 1
#	include <unistd.h>
#endif

// Shared lib / DLL special keywords if any
#ifdef PLATFORM_WINDOWS
#	include <win32/winver.h>
#	include <windows.h>

// MSVC and MinGW Windows DLL
#	ifdef CHANNEL_BUILD_DLL
#		define CHANNEL_PUBLIC __declspec(dllexport)
#	else
#		define CHANNEL_PUBLIC __declspec(dllimport)
#	endif

#else
// UNIX(ELF32/64)
#	define CHANNEL_PUBLIC
#endif

// Whether exception handling is disabled.
#if !defined(CHANNELS_NO_EXCEPTIONS)
# if defined(BOOST_ASIO_HAS_BOOST_CONFIG) && defined(BOOST_NO_EXCEPTIONS)
#  define CHANNELS_NO_EXCEPTIONS 1
# endif // !defined(BOOST_NO_EXCEPTIONS)
#endif // !defined(CHANNELS_NO_EXCEPTIONS)


#endif // CHANNELS_CONFIG_H_INCLUDED
