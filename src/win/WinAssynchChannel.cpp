#include "prchdrs.h"
#include "WinAssynchChannel.hpp"
#include "helpers.hpp"
#include "system.hpp"

namespace io {

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
WinAsynchChannel::WinAsynchChannel(::HANDLE handle):
	AsynchChannel(),
	object(),
	handle_(handle)
{}

void WinAsynchChannel::send(const byte_buffer& buffer,int64_t pos) const
{
	overlapped* ovpd = create_overlaped(SEND,buffer,pos);
	validate_io(::WriteFile(handle_, vpos(buffer), buffer.length(), NULL, ovpd),"Asynchronous sending failed");
}

void WinAsynchChannel::receive(byte_buffer& buffer,int64_t pos) const
{
	overlapped* ovpd = create_overlaped(RECEIVE,buffer,pos);
	validate_io(::ReadFile(handle_,vpos(buffer),buffer.length(),NULL,ovpd),"Asynchronous receiving failed ");
}

WinAsynchChannel::~WinAsynchChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
	::CloseHandle(handle_);
}

// WinAsynhDispatcher
WinAsynhDispatcher::WinAsynhDispatcher():
	AsynhDispatcher(),
	object(),
	port_(NULL)
{
	port_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
	validate_io(port_ != NULL,"Can not create asynchronous dispatcher");
}

WinAsynhDispatcher::~WinAsynhDispatcher() BOOST_NOEXCEPT_OR_NOTHROW
{
	::CloseHandle(port_);
}

void WinAsynhDispatcher::bind(SAsynchChannel channel)
{
	ULONG_PTR key = reinterpret_cast<ULONG_PTR>(channel.get());
	SWinAsynchChannel ch = boost::reinterpret_pointer_cast<WinAsynchChannel>(channel);
	keys_.push_back(ch);
	::HANDLE result = ::CreateIoCompletionPort(ch->descriptor(),port_,key,0);
	validate_io(result != port_,"Can not bind channel");
}

CopletitionEvent WinAsynhDispatcher::nextEvent()
{
	ULONG_PTR key;
	DWORD transfered = 0;
	::LPOVERLAPPED overlpd;
	validate_io(::GetQueuedCompletionStatus(port_,&transfered,&key,&overlpd,INFINITY),"Dispatcher error");
	overlapped *ovpd = reinterpret_cast<overlapped*>(overlpd);
	byte_buffer buff = ovpd->buffer;
	buff.move(transfered);
	EventType type = ovpd->type;
	AsynchChannel *ch = reinterpret_cast<WinAsynchChannel*>(key);
	delete ovpd;
	return CopletitionEvent(type,ch,buff);
}

SAsynhDispatcher CHANNEL_PUBLIC create_dispatcher()
{
	return SAsynhDispatcher(new WinAsynhDispatcher());
}

} // namespace io
