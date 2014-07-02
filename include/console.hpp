#ifndef CONSOLE_HPP_INCLUDED
#define CONSOLE_HPP_INCLUDED

#include <channels_config.h>

#include <boost/noncopyable.hpp>
#include <boost/config.hpp>
#include <convert.hpp>
#include <file.hpp>

#ifdef PLATFROM_WINDOWS
#	include <WindowsConsole.hpp>
#endif

namespace io {

#ifdef PLATFROM_WINDOWS

#if !defined(UNICODE_CONSOLE) && defined(UNICODE)
#	define UNICODE_CONSOLE true
#elif !defined(UNICODE_CONSOLE) && !defined(UNICODE)
#	define UNICODE_CONSOLE false
#endif

template<class ShannelType>
inline boost::shared_ptr<ShannelType> consChannel(DWORD handleId) {
	HANDLE hCons = ::GetStdHandle(handleId);
	ShannelType *ptr = new ShannelType(hCons,UNICODE_CONSOLE);
	return boost::shared_ptr<ShannelType>(ptr);
}

/**
 * ! \brief Factory for obtaining channels for windows standard streams.
 *  Standard in, out and error streams supported.
 */
class Console:private boost::noncopyable {
public:
	static void setCharset(const Charset* charset) BOOST_NOEXCEPT {
		::SetConsoleCP(charset->id());
		::SetConsoleOutputCP(charset->id());
	}
	static SWriteChannel outChanell() throw(std::bad_alloc) {
		return consChannel<ConsoleWriteChannel>(STD_OUTPUT_HANDLE);
	}
	static SReadChannel inChanell() throw(std::bad_alloc) {
		return consChannel<ConsoleReadChannel>(STD_INPUT_HANDLE);
	}
	static SWriteChannel errChanell() throw(std::bad_alloc) {
		return consChannel<ConsoleWriteChannel>(STD_OUTPUT_HANDLE);
	}
};
#endif // PLATFROM_WINDOWS

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
