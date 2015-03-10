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

#if defined(__unix__) || defined(__linux__)
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



#if defined(__GNUC__) && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || (__GNUC__ >= 5))
# define HACK_GCC_ITS_CPP0X 1
#endif

#if ((BOOST_VERSION / 100) % 1000) > 55
#   include <boost/atomic.hpp>
#else
#   define OLD_BOOST
#endif // NO Atomic

#ifdef OLD_BOOST

#define BOOST_MOVABLE_BUT_NOT_COPYABLE(TYPE)\
private:\
   TYPE(TYPE &);\
   TYPE& operator=(TYPE &);\
public:\
   operator const TYPE&() \
      {  return *static_cast< const TYPE* >(this);  }\
    operator const TYPE& () const \
      {  return *static_cast<const TYPE* >(this);  } \
private:

#define BOOST_NOEXCEPT_OR_NOTHROW throw()

#endif // OLD_BOOST

#endif // CHANNELS_CONFIG_H_INCLUDED
