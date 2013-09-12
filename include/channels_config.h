#ifndef CHANNELS_CONFIG_H_INCLUDED
#define CHANNELS_CONFIG_H_INCLUDED

#ifdef _WIN32
#	ifndef PLATFROM_WINDOWS
#		define PLATFROM_WINDOWS 1
#	endif
#	include <windows.h>
#endif

#ifdef __linux__
#	ifndef PLATFROM_LINUX
#		define PLATFROM_LINUX 1
#	endif
#endif

#if defined(__unix__)
#	define ANY_UNIX
#	include <unistd.h>
#endif

// Shared lib / DLL special keywords if any
#ifdef PLATFROM_WINDOWS
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
