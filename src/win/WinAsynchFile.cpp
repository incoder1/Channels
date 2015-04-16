#include "prchdrs.h"
#include "WinAsynchFile.hpp"
#include "helpers.hpp"

namespace io {

// WinAsyncResult
WinAsyncResult::WinAsyncResult(::HANDLE hFile,uint64_t position,const byte_buffer& buffer):
	AsyncResult(),
	object(),
	hFile_(hFile)
{
	::LARGE_INTEGER li;
	li.QuadPart = position;
	overlapped_->OffsetHigh = li.HighPart;
	overlapped_->Offset = li.LowPart;
	overlapped_->hEvent = reinterpret_cast<HANDLE>(this);
}

WinAsyncResult::~WinAsyncResult() BOOST_NOEXCEPT_OR_NOTHROW
{
}

std::size_t WinAsyncResult::await() {
	DWORD result;
	::GetOverlappedResult(hFile_,overlapped_.get(),&result,TRUE);
	return result;
}

virtual byte_buffer WinAsyncResult::buffer() {
	DWORD result;
	::GetOverlappedResult(hFile_,overlapped_.get(),&result,FALSE);
	buffer.move(result);
	return buffer_;
}

bool WinAsyncResult::cancel()
{
	active_ = ::CancelIoEx(hFile_,overlapped_.get());
	return active_;
}

bool WinAsyncResult::active() {
	return active_;
}

// WinAsychChannel
WinAsychChannel::WinAsychChannel(::HANDLE hFile,::HANDLE completionPort):
	AsynchRandomAccessChannel(),
	object(),
	hFile_(hFile),
	cmpltPort_(completionPort)
{
}

WinAsychChannel::~WinAsychChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
}

SAsyncResult WinAsychChannel::read(uint64_t pos,std::size_t maxbytes)
{
}

SAsyncResult WinAsychChannel::write(const byte_buffer& buffer)
{
	WinAsyncResult *aRes = new WinAsyncResult(hFile_,0,buffer);
 	SAsyncResult result(aRes);
 	DWORD errCode = ::GetLastError();
 	BOOL error = errCode != ERROR_IO_PENDING && errCode != ERROR_SUCCESS;
	validate_io(state && error, "Asynchronous write file error");
	return result;
}


} // namespace io
