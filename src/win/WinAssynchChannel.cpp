#include "prchdrs.h"
#include "WinAssynchChannel.hpp"
#include "helpers.hpp"

namespace io {

//AssynchChannel
AssynchChannel::AssynchChannel(SSelector selector):
	object(),
	selector_(selector)
{}

AssynchChannel::~AssynchChannel() BOOST_NOEXCEPT_OR_NOTHROW
{}

struct AssynchRequest:public ::OVERLAPPED {
	asynh_operation operation;
	void* buffer;
	std::size_t size;
};

AssynchRequest* make_request(asynh_operation operation,void* data,std::size_t size, uint64_t position) {
	AssynchRequest *result = new AssynchRequest;
	ZerroMemory(*result,sizeof(AssynchRequest));
	// set position
	::LARGE_INTEGER li;
	li.QuadPart = position;
	result->OffsetHi = li.HighPart;
	result->Offset = li.LowPart;
	// set operation
	result->operation = operation;
	result->buffer = data;
	result->size = size;
}

//WinAsynchChannel
WinAsynchChannel::WinAsynchChannel(::HANDLE hDevice):
	AssynchChannel(),
	hDevice_(hDevice)
{}

WinAsynchChannel::~WinAsynchChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
	::CloseHandle(hDevice);
}


void WinAsynchChannel::send(const byte_buffer& buff, uint64_t position) const
{
	uint8_t* data = ::VirtualAlloc(NULL, buff.length(), MEM_COMMIT, PAGE_READWRITE);
	std::copy( buff.position().ptr(), buff.last().ptr(), data);

	::WriteFile(hDevice_, data, buff.length(), NULL, ovlp);
}





// Selector
Selector::Selector():
		ioCnPrt_(INVALID_HANDLE_VALUE)
{
	ioCnPrt_ = ::CreateIoCompletionPort(ioCnPrt_,NULL,0,0);
	validate<std::runtime_error>(INVALID_HANDLE_VALUE != ioCnPrt_,"Can not create selector");
}


const AssynchChannel* Selector::bind(const AsynchDevice& device) {
	boost::unique_lock<boost::mutex> lock(mutex_);
	detail::WinAsynchChannel *ch = new detail::WinAsynchChannel(device.id());
}

void Selector::run() {
}

Selector::~Selector() BOOST_NOEXCEPT_OR_NOTHROW
{
	::CloseHandle(ioCnPrt_);
}


} // namespace io
