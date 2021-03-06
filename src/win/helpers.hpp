#ifndef HELPERS_HPP_INCLUDED
#define HELPERS_HPP_INCLUDED

#include "errors.hpp"

namespace io {

inline void* vpos(const byte_buffer& buff) {
	return reinterpret_cast<void*>(buff.begin().ptr());
}

/// Helper function used to throw char set error when expression
template<typename E>
void validate(bool expr, const std::string& msg) throw(E)
{
	if(!expr) {
		boost::throw_exception(E(msg));
	}
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
		result.append(" - ");
		result.append(lpMsgStr, lpMsgStr+bufLen);
		::LocalFree(lpMsgBuf);
	}
	return result;
}

inline void validate_io(BOOL expression, const char* failMessage)  {
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

}
#endif // HELPERS_HPP_INCLUDED
