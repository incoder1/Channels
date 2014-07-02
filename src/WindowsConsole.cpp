#include "prchdrs.h"
#include "WindowsConsole.hpp"

namespace io {


//ConsoleReadChannel
ConsoleReadChannel::ConsoleReadChannel(HANDLE hCons, bool wide) BOOST_NOEXCEPT_OR_NOTHROW:
	ReadChannel(),
	hCons_(hCons),
	peer_(wide? readf_t(ReadConsoleW) : readf_t(ReadConsoleA) ),
	charSize_(wide? sizeof(WCHAR) : sizeof(CHAR) )
{}

std::size_t ConsoleReadChannel::read(byte_buffer& buff) throw(io_exception)
{
	DWORD result;
	CONSOLE_READCONSOLE_CONTROL control;
	BOOL readRes = peer_(hCons_,vpos(buff),buff.capacity() / charSize_, &result, &control);
	buff.move(result*charSize_);
	validate_io(readRes, "Can't read from console" );
	return result;
}

ConsoleReadChannel::~ConsoleReadChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

//ConsoleWriteChannel
ConsoleWriteChannel::ConsoleWriteChannel(HANDLE hCons, bool unicode) BOOST_NOEXCEPT_OR_NOTHROW:
	WriteChannel(),
	hCons_(hCons),
	peer_(unicode ?  writef_t(WriteConsoleW) : writef_t(WriteConsoleA) ),
	charSize_(unicode? sizeof(WCHAR) : sizeof(CHAR) )
{}

std::size_t ConsoleWriteChannel::write(const byte_buffer& buffer) throw(io_exception)
{
	DWORD result;
	BOOL succeeded = peer_(hCons_,
	                vpos(buffer),
	                buffer.length() / charSize_,
	                &result,
	                NULL);
	validate_io(succeeded,"Write console error.");
	return result;
}

ConsoleWriteChannel::~ConsoleWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
}

} // namespace io
