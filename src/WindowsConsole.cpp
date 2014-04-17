#include "WindowsConsole.hpp"


namespace io {

// helpers
inline void* vpos(const byte_buffer& buff)
{
	return static_cast<void*>(&buff.position());
}

static std::string last_error_str(DWORD lastError)
{
	std::string result;
	LPVOID lpMsgBuf;
	static DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD bufLen = ::FormatMessage(flags,NULL,lastError,
	                               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	                               (LPTSTR) &lpMsgBuf, 0, NULL );
	if (bufLen) {
		LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
		result.append(lpMsgStr, lpMsgStr+bufLen);
		::LocalFree(lpMsgBuf);
	}
	return result;
}

inline void validate_io(BOOL ioResult, const char* message) throw(io_exception)
{
	if(!ioResult) {
		DWORD lastError = ::GetLastError();
		if(ERROR_IO_PENDING != lastError) {
			std::string errMsg(message);
			errMsg.append(" ");
			errMsg.append(last_error_str(lastError));
			boost::throw_exception(io_exception(errMsg));
		}
	}
}


//ConsoleReadChannel
ConsoleReadChannel::ConsoleReadChannel(HANDLE hCons, bool unicode) BOOST_NOEXCEPT_OR_NOTHROW:
	ReadChannel(),
	hCons_(hCons),
	peer_(unicode? readf_t(ReadConsoleW) : readf_t(ReadConsoleA) ),
	charSize_(unicode? sizeof(WCHAR) : sizeof(CHAR) )
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
