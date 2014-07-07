#ifndef HELPERS_HPP_INCLUDED
#define HELPERS_HPP_INCLUDED

#include <channels_config.h>

#include <boost/system/error_code.hpp>

namespace io {

inline void* vpos(const byte_buffer& buff) {
	return static_cast<void*>(buff.position().ptr());
}

#ifdef PLATFORM_WINDOWS

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

template<class Exception_T>
inline void validate(BOOL expression, const char* failMessage) throw(Exception_T) {
  if(!expression) {
		DWORD lastError = ::GetLastError();
		if(ERROR_IO_PENDING != lastError) {
			std::string errMsg(failMessage);
			errMsg.append(" ");
			errMsg.append(last_error_str(lastError));
			boost::throw_exception(Exception_T(errMsg));
		}
	}
}

inline void validate_io(BOOL ioResult, const char* message) throw(io_exception)
{
	validate<io_exception>(ioResult, message);
}

#endif // PLATFORM_WINDOWS

#ifdef PLATFORM_UNIX
inline void validate_io(ssize_t res, const char* msg) {
	if(-1 == res) {
		boost::system::error_code ec;
		std::string smsg(msg);
		smgs.append("");
		smgs.append(ec.category().what());
		boost::throw_exception(io_exception(smsg));
	}
}
#endif // PLATFROM UNIX

}
#endif // HELPERS_HPP_INCLUDED
