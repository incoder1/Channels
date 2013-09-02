#ifndef CHANNELS_CONFIG_H_INCLUDED
#define CHANNELS_CONFIG_H_INCLUDED

#if defined(_WIN32) || defined(_WIN64)
#	ifndef PLATFROM_WINDOWS
#		define PLATFROM_WINDOWS
#	endif
#endif

#ifdef PLATFROM_WINDOWS
#include <Windows.h>
// MSVC and MinGW
#	ifdef CHANNEL_BUILD_DLL
#		define CHANNEL_PUBLIC __declspec(dllexport)
#	else
#		define CHANNEL_PUBLIC __declspec(dllimport)
#	endif
#else
#	define CHANNEL_PUBLIC
#endif

#endif // CHANNELS_CONFIG_H_INCLUDED
