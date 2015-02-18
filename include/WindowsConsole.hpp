#ifndef WINDOWS_CONSOLE_HPP_INCLUDED
#define WINDOWS_CONSOLE_HPP_INCLUDED

#include <windows.h>

#include <boost/noncopyable.hpp>
#include <boost/config.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <convert.hpp>
#include <file.hpp>
#include <channels.hpp>

#ifndef BOOST_BIND_ENABLE_STDCALL
#	define BOOST_BIND_ENABLE_STDCALL
#endif

#ifndef BOOST_BIND_ENABLE_FASTCALL
#	define BOOST_BIND_ENABLE_FASTCALL
#endif

#if  defined(__MINGW32__) && !defined(_WIN64)
// MinGW64 have this structure in windows.h
typedef struct _CONSOLE_READCONSOLE_CONTROL {
	ULONG nLength;
	ULONG nInitialChars;
	ULONG dwCtrlWakeupMask;
	ULONG dwControlKeyState;
} CONSOLE_READCONSOLE_CONTROL, *PCONSOLE_READCONSOLE_CONTROL;
#endif /* __MINGW32__ */

// Use the const char* console version
// by default if WIDE_CONSOLE is not defined
#ifndef WIDE_CONSOLE
#	define WIDE_CONSOLE false
#endif

namespace io {

class CHANNEL_PUBLIC ConsoleReadChannel:public virtual ReadChannel, public virtual SmallObject
{
	private:
       typedef boost::function<BOOL(HANDLE,LPVOID,DWORD,LPDWORD,PCONSOLE_READCONSOLE_CONTROL)> readf_t;
	public:
		explicit ConsoleReadChannel(HANDLE hCons,bool unicode) BOOST_NOEXCEPT_OR_NOTHROW;
		virtual std::size_t read(byte_buffer& buffer) throw(io_exception);
		virtual ~ConsoleReadChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	private:
		HANDLE hCons_;
		readf_t peer_;
		std::size_t charSize_;
};

class CHANNEL_PUBLIC ConsoleWriteChannel:public virtual WriteChannel, public virtual SmallObject {
private:
	typedef boost::function<BOOL(HANDLE,const void*,DWORD,PDWORD,PVOID)> writef_t;
public:
	ConsoleWriteChannel(HANDLE hCons, bool unicode) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t write(const byte_buffer& buffer) throw(io_exception);
	virtual ~ConsoleWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW;
private:
	HANDLE hCons_;
	writef_t peer_;
	std::size_t charSize_;
};

namespace {

template<class ChannelType>
inline ChannelType* cons_ch(DWORD handleId, bool wide) {
	HANDLE hCons = ::GetStdHandle(handleId);
	return new ChannelType(hCons,wide);
}

inline SWriteChannel cwch(bool wide) {
	return SWriteChannel(cons_ch<ConsoleWriteChannel>(STD_OUTPUT_HANDLE,wide));
}
inline SReadChannel crch(bool wide) {
	return SReadChannel(cons_ch<ConsoleReadChannel>(STD_INPUT_HANDLE,wide));
}

inline SWriteChannel cech(bool wide) {
	return SWriteChannel(cons_ch<ConsoleWriteChannel>(STD_ERROR_HANDLE,wide));
}

} // namesapce


/**
 * ! \brief Factory for obtaining channels for windows standard streams.
 *  Standard in, out and error streams supported.
 */
class Console {
private:
	SWriteChannel cwch_;
	SReadChannel crch_;
	SWriteChannel cech_;
public:
	explicit Console(bool wide):
		cwch_(cwch(wide)),
		crch_(crch(wide)),
		cech_(cech(wide))
	{
		if(wide) {
			setCharset(Charsets::windowsUnicode());
		}
	}
	explicit Console():
		cwch_(cwch(WIDE_CONSOLE)),
		crch_(crch(WIDE_CONSOLE)),
		cech_(cech(WIDE_CONSOLE))
	{
		if(WIDE_CONSOLE) {
			setCharset(Charsets::windowsUnicode());
		}
	}
	Console(::HANDLE handle, bool wide):
		cwch_(SWriteChannel(new ConsoleWriteChannel(handle,wide))),
		crch_(SReadChannel(new ConsoleReadChannel(handle,wide))),
		cech_(SWriteChannel(new ConsoleWriteChannel(handle,wide)))
	{}
	inline void setCharset(const Charset* charset) BOOST_NOEXCEPT {
		::SetConsoleCP(charset->id());
		::SetConsoleOutputCP(charset->id());
	}
	inline void setCharset(const char* name) BOOST_NOEXCEPT_OR_NOTHROW {
		setCharset(Charsets::forName(name));
	}
	inline SWriteChannel outChanell() const {
		return cwch_;
	}
	inline SReadChannel inChanell() const {
		return crch_;
	}
	inline SWriteChannel errChanell() const {
		return cech_;
	}
};

} // namespace io

#endif // WINDOWS_CONSOLE_HPP_INCLUDED
