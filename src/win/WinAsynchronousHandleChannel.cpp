#include "prchdrs.h"
#include "WinAsynchronousHandleChannel.hpp"
#include "helpers.hpp"

namespace io {

WinAsynchronousHandleChannel::WinAsynchronousHandleChannel(boost::asio::io_service& ios,::HANDLE hDevice):
	ios_(ios),
	hDevice_(hDevice)
{}

WinAsynchronousHandleChannel::~WinAsynchronousHandleChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
	::CloseHandle(hDevice_);
}

void WinAsynchronousHandleChannel::send(std::size_t position,byte_buffer& buffer,const asynh_handler_f& handler) BOOST_NOEXCEPT_OR_NOTHROW
{
	// Add object reference to make handle closed only when all io operations were complete
	SAsynchronousChannel intr(reinterpret_cast<AsynchronousChannel*>(this), true);
	boost::asio::windows::overlapped_ptr overlapped(ios_, boost::bind(handler, intr, _1, _2, buffer) );
	::LPOVERLAPPED povd = overlapped.get();
	::LARGE_INTEGER li;
	li.QuadPart = position;
	povd->Offset = li.LowPart;
	povd->OffsetHigh = li.HighPart;
	::DWORD transfered;
	::BOOL status = ::WriteFile(hDevice_,vpos(buffer),buffer.length(),&transfered,povd);
	// Check if the operation failed
	::DWORD lastError = ::GetLastError();
	if (!status && lastError != ERROR_IO_PENDING) {
		// call handler with error
		error_code err(lastError, boost::asio::error::get_system_category());
		overlapped.complete(err, transfered);
	} else {
		// The operation was successfully initiated, so ownership of the
		// OVERLAPPED-derived object has passed to the io_service.
		overlapped.release();
	}
}

} // namesapce io
