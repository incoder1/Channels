#ifndef CONSOLE_HPP_INCLUDED
#define CONSOLE_HPP_INCLUDED

#include <channels_config.h>

#include <boost/noncopyable.hpp>
#include <boost/config.hpp>
#include <convert.hpp>
#include <file.hpp>

namespace io {

#ifdef PLATFROM_WINDOWS
/**
 * ! \brief Factory for obtaining channels for windows standard streams.
 *  Standard in, out and error stream supported.
 */
class Console:private boost::noncopyable
{
	public:
		static void setCharset(const Charset* charset) BOOST_NOEXCEPT {
			::SetConsoleCP(charset->id());
			::SetConsoleOutputCP(charset->id());
		}
		static PWriteChannel outChanell() throw(std::bad_alloc) {
			return PWriteChannel(new FileChannel(::GetStdHandle(STD_OUTPUT_HANDLE)));
		}
		static PReadChannel inChanell() throw(std::bad_alloc) {
			return PReadChannel(new FileChannel(::GetStdHandle(STD_INPUT_HANDLE)));
		}
		static PWriteChannel errChanell() throw(std::bad_alloc) {
			return PWriteChannel(new FileChannel(::GetStdHandle(STD_ERROR_HANDLE)));
		}
};
#endif // PLATFROM_WINDOWS

#ifdef PLATFORM_UNIX
/**
 * ! \brief Factory for obtaining channels for UNIX standard streams.
 *  Standard in, out and error stream supported.
 */
class Console:private boost::noncopyable
{
	public:
		static PWriteChannel outChanell() throw(std::bad_alloc) {
			return PWriteChannel(new FileChannel(stdout));
		}
		static PReadChannel inChanell() throw(std::bad_alloc) {
			return PReadChannel(new FileChannel(stdin));
		}
		static PWriteChannel errChanell() throw(std::bad_alloc) {
			return PWriteChannel(new FileChannel(stderr));
		}
};
#endif // PLATFORM_UNIX

} // namespace io

#endif // CONSOLE_HPP_INCLUDED
