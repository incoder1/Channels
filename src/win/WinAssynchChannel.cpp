#include "prchdrs.h"
#include "WinAssynchChannel.hpp"
#include "helpers.hpp"
#include "system.hpp"

namespace io {

typedef bool EventType;

const EventType SEND = true;
const EventType RECV = false;

struct overlapped:public ::OVERLAPPED
{
public:
	overlapped(EventType t,const byte_buffer& buff):
		type(t),
		buffer(buff)
	{
		Internal = 0;
		InternalHigh = 0;
		Offset = 0;
		OffsetHigh = 0;
		Pointer = NULL;
		hEvent = NULL;
	}
public:
	EventType type;
	byte_buffer buffer;
};

static inline overlapped* create_overlaped(EventType type,const byte_buffer& buff,int64_t pos) {
	overlapped *result = new overlapped(type,buff);
	::LARGE_INTEGER li;
	li.QuadPart = pos;
	result->OffsetHigh = li.HighPart;
	result->Offset = li.LowPart;
	result->type = type;
	return result;
}

// AsynchChannel
WinAsynchChannel::WinAsynchChannel(::HANDLE handle,const completion_handler_f& recvHnd,const completion_handler_f& sndHnd):
	AsynchChannel(recvHnd,sndHnd),
	object(),
	handle_(handle)
{
	validate<io_exception>(handle_ != INVALID_HANDLE_VALUE,"Invalid handle provided");
}

void WinAsynchChannel::send(const byte_buffer& buffer,int64_t pos) const
{
	overlapped* ovpd = create_overlaped(SEND,buffer,pos);
	validate_io(::WriteFile(handle_, vpos(buffer), buffer.length(), NULL, ovpd),"Asynchronous sending failed");
}

void WinAsynchChannel::receive(byte_buffer& buffer,int64_t pos) const
{
	overlapped* ovpd = create_overlaped(RECV,buffer,pos);
	validate_io(::ReadFile(handle_,vpos(buffer),buffer.length(),NULL,ovpd),"Asynchronous receiving failed");
}


WinAsynchChannel::~WinAsynchChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
	::CloseHandle(handle_);
}

// WinAsynhDispatcher
WinAsynhDispatcher::WinAsynhDispatcher(std::size_t maxThreads):
	AsynhDispatcher(),
	object(),
	maxThreads_(maxThreads),
	port_(NULL)
{
	port_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,maxThreads_);
	validate_io(port_ != NULL,"Can not create asynchronous dispatcher");
}

WinAsynhDispatcher::~WinAsynhDispatcher() BOOST_NOEXCEPT_OR_NOTHROW
{
	pool_.join_all();
	::CloseHandle(port_);
}

void WinAsynhDispatcher::bind(SAsynchChannel channel)
{
	SWinAsynchChannel ch = boost::reinterpret_pointer_cast<WinAsynchChannel>(channel);
	keys_.push_back(ch);
	ULONG_PTR key = reinterpret_cast<ULONG_PTR>(channel.get());
	::HANDLE result = ::CreateIoCompletionPort(ch->descriptor(),port_,key,maxThreads_);
	validate_io(result != port_,"Can not bind channel");
}

void WinAsynhDispatcher::routine()
{
	WinAsynchChannel *channel;
	DWORD transfered = 0;
	::LPOVERLAPPED overlpd;
//	do {
//		validate_io(::GetQueuedCompletionStatus(port_,&transfered,(PULONG_PTR)&channel,&overlpd,INFINITY),"Dispatcher error");
//	} while(overlpd->event != SEND);
	boost::scoped_ptr<overlapped> ovpd(reinterpret_cast<overlapped*>(overlpd));

	byte_buffer buff = ovpd->buffer;
	buff.move(transfered);

	if(ovpd->type) {
		channel->handleSend(transfered,buff);
	} else {
		channel->handleReceive(transfered,buff);
	}
}

void WinAsynhDispatcher::start()
{
	for(std::size_t i=0; i<maxThreads_; i++) {
		pool_.create_thread(boost::bind(&WinAsynhDispatcher::routine,this));
	}
}

SAsynhDispatcher CHANNEL_PUBLIC create_dispatcher(std::size_t maxThreads)
{
	return SAsynhDispatcher(new WinAsynhDispatcher(maxThreads));
}

} // namespace io
