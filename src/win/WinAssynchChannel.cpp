#include "prchdrs.h"
#include "WinAssynchChannel.hpp"
#include "helpers.hpp"
#include "system.hpp"

#ifndef ERROR_ABANDONED_WAIT_0
#	define ERROR_ABANDONED_WAIT_0 0x2DF
#endif // ERROR_ABANDONED_WAIT_0

namespace io {

WinSelector::WinSelector(uint32_t maxThreads):
	port_(NULL),
	maxThreads_(maxThreads)
{
	port_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, port_, NULL, maxThreads_);
	validate_io(NULL != port_, "System error, can't create selector");
	if(0 == maxThreads_) {
		maxThreads_ = sys::available_logical_cpus();
	}
}

void WinSelector::bind(::HANDLE handle, ULONG_PTR key) const
{
	::HANDLE res = ::CreateIoCompletionPort(
					handle,
					port_,
					key,
					maxThreads_);
	validate_io(res == port_, "System error, can not bind device");
}

WinSelector::~WinSelector() BOOST_NOEXCEPT_OR_NOTHROW
{
	::CloseHandle(port_);
}

void WinSelector::listen_routine(::HANDLE port) {
	detail::Overlapped *overlapped;
	WinAsynchChannel const *channel;
	DWORD transfered;
	for(;;) {
		BOOLEAN status = ::GetQueuedCompletionStatus(
				port,
				&transfered,
				reinterpret_cast<PULONG_PTR>(&channel),
				(reinterpret_cast<LPOVERLAPPED*>(&overlapped)),
				INFINITY);
		if(!status) {
			DWORD lastError = ::GetLastError();
			if(lastError != ERROR_ABANDONED_WAIT_0) {
				boost::throw_exception(std::runtime_error("System error"+last_error_str(::GetLastError())));
			} else {
				// should not happens
				break;
			}
			if(overlapped->operation == detail::SEND) {
				channel->handleSendDone(transfered,overlapped->buffer);
			}
			delete overlapped;
		}
	}
}


void WinSelector::start() {
	std::vector<boost::thread> pool(maxThreads_);
	for(uint32_t i=0; i < maxThreads_; i++) {
		pool.push_back(boost::thread(boost::bind(&WinSelector::listen_routine, port_)));
	}
}

WinAsynchChannel::WinAsynchChannel(const WinSelector* selector,::HANDLE handle,const completition_handler_f& sendHandler):
	AsynchChannel(sendHandler),
	object(),
	handle_(handle)
{
	selector->bind(handle,reinterpret_cast<ULONG_PTR>(this));
}

void WinAsynchChannel::handleSendDone(std::size_t transfered, byte_buffer& buf) const
{
	boost::system::error_code ec(::GetLastError(), boost::system::generic_category());
	buf.move(transfered);
	handleSend(ec,transfered,buf);
}

WinAsynchChannel::~WinAsynchChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
	::CloseHandle(handle_);
}

void WinAsynchChannel::send(uint64_t offset,const byte_buffer& buffer)
{
	detail::Overlapped *overlapped = new detail::Overlapped(detail::SEND,offset,buffer);
	::WriteFile(handle_, vpos(buffer), buffer.length(), NULL, overlapped);
}

// Selector

} // namespace io
