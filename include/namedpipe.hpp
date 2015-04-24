#ifndef __IO_NAMEDPIPE_HPP_INCLUDED__
#define __IO_NAMEDPIPE_HPP_INCLUDED__

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>

#include "channels.hpp"
#include "smallobject.hpp"

namespace io {

typedef boost::function<void(long,std::size_t,byte_buffer&)> completition_handler;

class AsynchChannel {
	BOOST_MOVABLE_BUT_NOT_COPYABLE(AsynchChannel)
protected:
	AsynchChannel()
	{}
public:
	virtual ~AsynchChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
	virtual bool receive(std::size_t max, int64_t offset) = 0;
	virtual bool send(const byte_buffer& buff,int64_t offset) = 0;
};

DECLARE_PTR_T(AsynchChannel);

class AsynchRequest:public ::OVERLAPPED {
public:
	AsynchRequest(int64_t pos,const byte_buffer& buff):
		buffer_(buff) {
		Internal = Offset = OffsetHigh = 0;
		hEvent = NULL;
		LARGE_INTEGER li;
		li.QuadPart = pos;
		OffsetHigh = li.HighPart;
		Offset = li.LowPart;
	}
	inline byte_buffer buffer() const {
		return buffer_;
	}
private:
	byte_buffer buffer_;
};


class WinAsynchChannel:public virtual AsynchChannel,public virtual object {
public:
	WinAsynchChannel(::HANDLE handle,const completition_handler& handler):
		AsynchChannel() {
	}
	~WinAsynchChannel() BOOST_NOEXCEPT_OR_NOTHROW {
		::CloseHandle(id_);
	}
	virtual bool send(std::size_t max, int64_t offset) {
		byte_buffer buff = byte_buffer::heap_buffer(max);
		AsynchRequest *request = new AsynchRequest(offset,buff);
		//	::ReadFileEx(id_,vpos(buff),max,NULL,request);
	}
	virtual bool send(const byte_buffer& buff,int64_t offset) {
		AsynchRequest *request = new AsynchRequest(offset,buff);
		//	::WriteFileEx(id_,vpos(buff),buff.length(),NULL,request);
	}
	::HANDLE systemId() const {
		return id_;
	}
private:
	::HANDLE id_;
};

class Selector {
public:
	Selector():
		port_(NULL) {
		port_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,0);
	}
	~Selector() BOOST_NOEXCEPT_OR_NOTHROW {
		::CloseHandle(port_);
	}
	void bind(SAsynchChannel channel) {
		WinAsynchChannel *ch = reinterpret_cast<WinAsynchChannel*>(channel.get());
		::HANDLE handle = ch->systemId();
		::CreateIoCompletionPort(handle,port_,ch->hash(),0);
	}
private:
	boost::unordered_map<DWORD,WinAsynchChannel*> channels_;
	::HANDLE port_;
};


} // namesapce io

#endif // __IO_NAMEDPIPE_HPP_INCLUDED__
