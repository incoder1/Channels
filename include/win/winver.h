#ifndef __IO_WINVER_H_INCLUDED__
#define __IO_WINVER_H_INCLUDED__

#ifdef _WIN32_WINNT
#	undef WINVER
#endif

#ifdef _WIN32_WINNT
#	undef _WIN32_WINNT
#endif

#ifdef _WIN32_WINDOWS
#	undef _WIN32_WINDOWS
#endif

#define WINVER 0x0600
#define _WIN32_WINNT 0x0600
#define _WIN32_WINDOWS 0x0600

#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#	define NOMINMAX
#endif

#ifndef _USE_MATH_DEFINES
#	define _USE_MATH_DEFINES
#endif

#endif // __IO_WINVER_H_INCLUDED__
