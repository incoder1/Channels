#ifndef CHANNEL_PIPE_HPP_INCLUDED
#define CHANNEL_PIPE_HPP_INCLUDED

#include <channels_config.h>

#ifdef PLATFORM_WINDOWS
#	include "WindowsPipe.hpp"
namespace io {
	typedef WindowsPipe Pipe;
} // namespace io
#elif defined(PLATFROM_UNIX)
// TODO: Implement me
#endif

#endif // CHANNEL_PIPE_HPP_INCLUDED
