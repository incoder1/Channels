#include "prchdrs.h"
#include "WinAsynchFile.hpp"
#include "helpers.hpp"

namespace io {

// WinAsyncResult
WinAsyncResult::WinAsyncResult(::HANDLE hFile,
							::LPOVERLAPPED overlapped,
							const completion_routine_f& routine):
	AsyncResult(),
	object(),
	hFile_(hFile),
	overlapped_(overlapped_),
	routine_(routine),
	active_(true)
{
	overlaped_.hEvent = reinterpret_cast<HANDLE>(this);
}

WinAsyncResult::~WinAsyncResult() BOOST_NOEXCEPT_OR_NOTHROW
{
}

inline LPOVERLAPPED WinAsyncResult::overlaped() {
	return overlaped_;
}

VOID CALLBACK WinAsyncResult::done_routine(DWORD errCode,DWORD transfered, LPOVERLAPPED overlapped)
{
	WinAsyncResult* self = reinterpret_cast<WinAsyncResult*>(overlapped->hEvent);
	self->onComplete(errCode,transfered);
}

std::size_t WinAsyncResult::await() {
	DWORD result;
	::GetOverlappedResult(hFile_,overlaped_,&result,TRUE);
	return result;
}

bool WinAsyncResult::cancel()
{
	atctive_ = ::CancelIo(hFile_);
	return active_;
}

inline void onComplete(int errorCode,std::size_t transfered)
{
	active_ = false;
	routine_(errorCode,transfered);
}

bool WinAsyncResult::active() {
	return active_;
}

// WinAsychFileChannel
WinAsychFileChannel::WinAsychFileChannel(::HANDLE hFile):
	AsynchReadWriteChannel(),
	object(),
	hFile_(hFile),
	counter
{
}

SAsyncResult WinAsychFileChannel::write(const byte_buffer& buffer)
{
	SAsyncResult asyncResult = boost::make_shared<WinAsyncResult>(hFile_);
 	BOOL result = ::WriteFileEx(*hFile_,vpos(buff),buffer.length(),asyncResult->overlaped(),&WinAsyncResult::done_routine);
	validate_io(result && ERROR_SUCCESS == ::GetLastError(), "Asynchronous write file error");
	return asyncResult;
}

WinAsychFileChannel::~WinAsychFileChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
	::GetOverlappedResultEx();
}

} // namespace io
