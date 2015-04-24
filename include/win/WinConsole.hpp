#ifndef WINDOWS_CONSOLE_HPP_INCLUDED
#define WINDOWS_CONSOLE_HPP_INCLUDED

#include "winver.h"
#include <windows.h>

#include <boost/noncopyable.hpp>
#include <boost/config.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <tchar.h>
#include <text.hpp>

#include "smallobject.hpp"

#if  defined(__MINGW32__) && !defined(_WIN64)
// MinGW64 have this structure in windows.h
typedef struct _CONSOLE_READCONSOLE_CONTROL {
	ULONG nLength;
	ULONG nInitialChars;
	ULONG dwCtrlWakeupMask;
	ULONG dwControlKeyState;
} CONSOLE_READCONSOLE_CONTROL, *PCONSOLE_READCONSOLE_CONTROL;
#endif /* __MINGW32__ */


namespace io {

class CHANNEL_PUBLIC ConsoleReadChannel:public virtual ReadChannel, public virtual object
{
	private:
       typedef boost::function<BOOL(HANDLE,LPVOID,DWORD,LPDWORD,PCONSOLE_READCONSOLE_CONTROL)> readf_t;
	public:
		explicit ConsoleReadChannel(HANDLE hCons,bool unicode) BOOST_NOEXCEPT_OR_NOTHROW;
		virtual std::size_t read(byte_buffer& buffer);
		virtual ~ConsoleReadChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	private:
		HANDLE hCons_;
		readf_t peer_;
		std::size_t charSize_;
};

class CHANNEL_PUBLIC ConsoleWriteChannel:public virtual WriteChannel, public virtual object {
private:
	typedef boost::function<BOOL(HANDLE,const void*,DWORD,PDWORD,PVOID)> writef_t;
public:
	ConsoleWriteChannel(HANDLE hCons, bool unicode) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t write(const byte_buffer& buffer) ;
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

const uint8_t BLACK        = 0x0;
const uint8_t NAVY_BLUE    = 0x1;
const uint8_t NAVY_GREEN   = 0x2;
const uint8_t NAVY_AQUA    = 0x3;
const uint8_t NAVY_RED     = 0x4;
const uint8_t NAVY_PURPLE  = 0x5;
const uint8_t NAVY_YELLOW  = 0x6;
const uint8_t WHITE        = 0x7;
const uint8_t GREY         = 0x8;
const uint8_t LIGHT_BLUE   = 0x9;
const uint8_t LIGHT_GREEN  = 0xA;
const uint8_t LIGHT_AQUA   = 0xB;
const uint8_t LIGHT_RED    = 0xC;
const uint8_t LIGHT_PURPLE = 0xD;
const uint8_t LIGHT_YELLOW = 0xE;
const uint8_t BRIGHT_WHITE = 0xF;

/**
 * ! \brief Factory for obtaining channels for windows standard streams.
 *  Standard in, out and error streams supported.
 */
class Console {
public:
	explicit Console(bool wide):
		charSet_( wide ? Charsets::utf16le() : Charsets::forId(::GetConsoleCP()) ),
		cwch_(cwch(wide)),
		crch_(crch(wide)),
		cech_(cech(wide))
	{
		if(wide) {
			setCharset(charSet_);
		}
	}
	Console(::HANDLE handle, bool wide):
		charSet_( wide ? Charsets::utf16le() : Charsets::forId(::GetConsoleCP()) ),
		cwch_(SWriteChannel(new ConsoleWriteChannel(handle,wide))),
		crch_(SReadChannel(new ConsoleReadChannel(handle,wide))),
		cech_(SWriteChannel(new ConsoleWriteChannel(handle,wide)))
	{
		if(wide) {
			setCharset(charSet_);
		}
	}
	inline void setCharset(const Charset* charset) BOOST_NOEXCEPT_OR_NOTHROW {
		charSet_ = charset;
		::SetConsoleCP(charset->id());
		::SetConsoleOutputCP(charset->id());
	}
	inline void setCharset(const char* name) BOOST_NOEXCEPT_OR_NOTHROW {
		setCharset(Charsets::forName(name));
	}
	inline const Charset* charset() {
		return charSet_;
	}
	inline SWriteChannel out() const {
		return cwch_;
	}
	inline SReadChannel in() const {
		return crch_;
	}
	inline SWriteChannel err() const {
		return cech_;
	}
private:
	const Charset* charSet_;
	SWriteChannel cwch_;
	SReadChannel crch_;
	SWriteChannel cech_;
};

} // namespace io

#endif // WINDOWS_CONSOLE_HPP_INCLUDED
