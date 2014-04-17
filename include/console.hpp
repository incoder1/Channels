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

#ifdef UNICODE
#	define UNICODE_CONS true
#else
#	define UNICODE_CONS false
#endif

template<class ShannelType>
inline boost::shared_ptr<ShannelType> consChannel(DWORD handleId) {
	HANDLE hCons = ::GetStdHandle(handleId);
	ShannelType *ptr = new ShannelType(hCons,UNICODE_CONS);
	return boost::shared_ptr<ShannelType>(ptr);
}

/**
 * ! \brief Factory for obtaining channels for windows standard streams.
 *  Standard in, out and error stream supported.
 */
class Console:private boost::noncopyable {
private:

	static inline FileChannel* consCl(DWORD id) {
		return new FileChannel(::GetStdHandle(id),false);
	}
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
 *  Standard in, out and error stream supported.
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
