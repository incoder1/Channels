#ifndef CONSOLE_HPP_INCLUDED
#define CONSOLE_HPP_INCLUDED

#include "channels_config.h"

#ifdef PLATFORM_WINDOWS
#	include "win/WinConsole.hpp"
#elif PLATFORM_UNIX
#   include "unix/unixconsole.hpp"
#endif

#endif // CONSOLE_HPP_INCLUDED
