#ifndef WINDOWS_CONSOLE_HPP_INCLUDED
#define WINDOWS_CONSOLE_HPP_INCLUDED

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <channels.hpp>

#ifndef BOOST_BIND_ENABLE_STDCALL
#	define BOOST_BIND_ENABLE_STDCALL
#endif

#ifndef BOOST_BIND_ENABLE_FASTCALL
#	define BOOST_BIND_ENABLE_FASTCALL
#endif

#ifdef __GNUC__
typedef struct _CONSOLE_READCONSOLE_CONTROL {
	ULONG nLength;
	ULONG nInitialChars;
	ULONG dwCtrlWakeupMask;
	ULONG dwControlKeyState;
} CONSOLE_READCONSOLE_CONTROL, *PCONSOLE_READCONSOLE_CONTROL;
#endif /* __GNUC__ */

namespace io {

class CHANNEL_PUBLIC ConsoleReadChannel:public ReadChannel
{
	private:
		typedef boost::function<BOOL (HANDLE,LPVOID,DWORD,LPDWORD,PCONSOLE_READCONSOLE_CONTROL) > readf_t;
	public:
		explicit ConsoleReadChannel(HANDLE hCons,bool unicode) BOOST_NOEXCEPT_OR_NOTHROW;
		virtual std::size_t read(byte_buffer& buffer) throw(io_exception);
		virtual ~ConsoleReadChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	private:
		HANDLE hCons_;
		readf_t peer_;
		std::size_t charSize_;
};

class CHANNEL_PUBLIC ConsoleWriteChannel:public WriteChannel {
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

} // namespace io

#endif // WINDOWS_CONSOLE_HPP_INCLUDED
