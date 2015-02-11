#ifndef HELPERS_HPP_INCLUDED
#define HELPERS_HPP_INCLUDED

#include <channels_config.h>
#include <boost/system/error_code.hpp>

namespace io {

inline void* vpos(const byte_buffer& buff) {
	return static_cast<void*>(buff.position().ptr());
}

template<typename R>
inline R vpos(const byte_buffer& buff) {
	return reinterpret_cast<R>(buff.position().ptr());
}

/// Helper function used to throw char set error when expression
template<typename E>
void validate(bool expr, const std::string& msg) throw(E)
{
	if(!expr) {
		boost::throw_exception(E(msg));
	}
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
		result.append(" - ");
		result.append(lpMsgStr, lpMsgStr+bufLen);
		::LocalFree(lpMsgBuf);
	}
	return result;
}

inline void validate_io(BOOL expression, const char* failMessage) throw(io_exception) {
  if(!expression) {
		DWORD lastError = ::GetLastError();
		if(ERROR_IO_PENDING != lastError) {
			std::string errMsg(failMessage);
			errMsg.append(" ");
			errMsg.append(last_error_str(lastError));
			boost::throw_exception(io_exception(errMsg));
		}
	}
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
