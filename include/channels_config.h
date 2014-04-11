#ifndef CHANNELS_CONFIG_H_INCLUDED
#define CHANNELS_CONFIG_H_INCLUDED4

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	define MSVCCMP
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
#	ifndef PLATFROM_WINDOWS
#		define PLATFROM_WINDOWS 1
#	endif
#endif

#if defined(__unix__)
#	define PLATFORM_UNIX 1
#	include <unistd.h>
#endif

// Shared lib / DLL special keywords if any
#ifdef PLATFROM_WINDOWS

#	ifdef CHANNEL_BUILD_DLL
#		define WIN32_LEAN_AND_MEAN
#	endif // WIN32_LEAN_AND_MEAN
#	include <winsock2.h>
#	include <windows.h>

// MSVC and MinGW
#	ifdef CHANNEL_BUILD_DLL
#		define CHANNEL_PUBLIC __declspec(dllexport)
#	else
#		define CHANNEL_PUBLIC __declspec(dllimport)
#	endif

#else
// unixes does not need anything special
#	define CHANNEL_PUBLIC
#endif

#endif // CHANNELS_CONFIG_H_INCLUDED
