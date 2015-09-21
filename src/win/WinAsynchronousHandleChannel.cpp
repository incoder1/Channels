#include "prchdrs.h"
#include "WinAsynchronousHandleChannel.hpp"
#include "helpers.hpp"

namespace io {

WinAsynchronousHandleChannel::WinAsynchronousHandleChannel(boost::asio::io_service& ios,::HANDLE hDevice):
	hDevice_(ios),
	ios_(ios)
{
	boost::system::error_code err;
	hDevice_.assign(hDevice, err);
	if(err && !hDevice_.is_open()) {
		boost::throw_exception(io_exception(err.message()));
	}
}

WinAsynchronousHandleChannel::~WinAsynchronousHandleChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

void WinAsynchronousHandleChannel::seek(std::size_t pos,::LPOVERLAPPED overlapped)
{
	::LARGE_INTEGER li;
	li.QuadPart = pos;
	overlapped->Offset = li.LowPart;
	overlapped->OffsetHigh = li.HighPart;
}

void WinAsynchronousHandleChannel::receive(std::size_t position,std::size_t maxSize,const handler_f& handler)
{
	byte_buffer buffer = byte_buffer::heap_buffer(maxSize);
	SAsyncChannel intr(dynamic_cast<AsyncChannel*>(this));
	boost::asio::windows::overlapped_ptr overlapped(ios_,
													boost::bind(
															handler,
															intr,
															boost::asio::placeholders::error,
															boost::asio::placeholders::bytes_transferred,
															buffer
														)
													);
	seek(position,overlapped.get());

	::DWORD transfered;
	::BOOL status = ::ReadFile(hDevice_.native_handle(), vpos(buffer), maxSize, &transfered, overlapped.get());
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

void WinAsynchronousHandleChannel::send(std::size_t position,byte_buffer& buffer,const handler_f& handler)
{
	SAsyncChannel intr(dynamic_cast<AsyncChannel*>(this));
	boost::asio::windows::overlapped_ptr overlapped(ios_,
													boost::bind(
															handler,
															intr,
															boost::asio::placeholders::error,
															boost::asio::placeholders::bytes_transferred,
															buffer
														)
													);
	seek(position,overlapped.get());

	::DWORD transfered;
	::BOOL status = ::WriteFile(hDevice_.native_handle(),vpos(buffer),buffer.length(),&transfered,overlapped.get());
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
