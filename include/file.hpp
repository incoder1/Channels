#ifndef FILE_HPP_INCLUDED
#define FILE_HPP_INCLUDED

#include "channels_config.h"

#ifdef PLATFORM_WINDOWS
#	include "win/WinFile.hpp"
#elif defined(PLATFORM_UNIX)
#	include "unix/unixfile.hpp"
#else
#	error "This operating system is not supported"
#endif // PLATFORM_UNIX

#endif // FILE_HPP_INCLUDED
