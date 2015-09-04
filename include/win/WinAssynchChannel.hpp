#ifndef __WIN_ASSYNCH_CHANNEL_HPP_INCLUDED__
#define __WIN_ASSYNCH_CHANNEL_HPP_INCLUDED__

#include "channels.hpp"

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

namespace io {

namespace detail {

	enum AsynchOperation {
		SEND, RECAIVE
	};

	struct Overlapped: public OVERLAPPED
	{
		AsynchOperation operation;
		byte_buffer buffer;
		Overlapped(AsynchOperation operation,uint64_t position, const byte_buffer& buff) BOOST_NOEXCEPT_OR_NOTHROW:
			operation(operation),
			buffer(buff)
		{
			Internal = static_cast<ULONG_PTR>(NULL);
			InternalHigh = static_cast<ULONG_PTR>(NULL);
			Pointer = NULL;
			hEvent = static_cast<::HANDLE>(NULL);
			// set-up offset
			::LARGE_INTEGER li;
			li.QuadPart = position;
			OffsetHigh = li.HighPart;
			Offset = li.LowPart;
		}
	};

}

class CHANNEL_PUBLIC WinSelector:private boost::noncopyable
{
private:
	static void listen_routine(::HANDLE port);
public:
	WinSelector(uint32_t maxThreads);
	~WinSelector();
	void start();
	void bind(::HANDLE handle,ULONG_PTR key) const;
private:
	::HANDLE port_;
	boost::thread_group pool_;
	uint32_t maxThreads_;
};

class CHANNEL_PUBLIC WinAsynchChannel:public virtual AsynchChannel,public virtual object {
public:
	explicit WinAsynchChannel(const WinSelector* selector,::HANDLE handle,const completition_handler_f& sendHandler);
	virtual ~WinAsynchChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual void send(uint64_t offset,const byte_buffer& buffer);
	void handleSendDone(DWORD lastError,std::size_t transfered, byte_buffer& buff) const;
private:
	::HANDLE handle_;
};


} // namespace io


#endif // __WIN_ASSYNCH_CHANNEL_HPP_INCLUDED__
