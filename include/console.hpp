#ifndef CONSOLE_HPP_INCLUDED
#define CONSOLE_HPP_INCLUDED

#include <channels_config.h>

#ifdef PLATFROM_WINDOWS
#	include <WindowsConsole.hpp>
#endif

namespace io {


#ifdef PLATFORM_UNIX
/**
 * ! \brief Factory for obtaining channels for UNIX standard streams.
 *  Standard in, out and error streams supported.
 */
class Console:private boost::noncopyable {
public:
	static SWriteChannel outChanell() throw(std::bad_alloc) {
		return SWriteChannel(new FileChannel(stdout));
	}
	static SReadChannel inChanell() throw(std::bad_alloc) {
		return SReadChannel(new FileChannel(stdin));
	}
	static SWriteChannel errChanell() throw(std::bad_alloc) {
		return SWriteChannel(new FileChannel(stderr));
	}
};
#endif // PLATFORM_UNIX

} // namespace io

#endif // CONSOLE_HPP_INCLUDED
