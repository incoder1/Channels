#ifndef __WIN_ASSYNCH_CHANNEL_HPP_INCLUDED__
#define __WIN_ASSYNCH_CHANNEL_HPP_INCLUDED__

#include "channels.hpp"

#include <boost/noncopyable.hpp>

#include <boost/thread/executors/basic_thread_pool.hpp>
#include <boost/thread/future.hpp>
#include <boost/thread/thread.hpp>

#include <boost/unordered_map.hpp>

#include <vector>
#include <windows.h>

namespace io {

class CHANNEL_PUBLIC WinAsynchChannel:public virtual AsynchChannel,public virtual object {
public:
	WinAsynchChannel(::HANDLE handle,const completition_handler);
	~WinAsynchChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual void send(const byte_buffer& buffer,int64_t pos) const;
	virtual void receive(byte_buffer& buffer,int64_t pos) const;
	BOOST_FORCEINLINE ::HANDLE descriptor() const {
		return handle_;
	}
private:
	::HANDLE handle_;
};

DECLARE_PTR_T(WinAsynchChannel);

class WinAsynhDispatcher:public virtual AsynhDispatcher,public virtual object
{
public:
	WinAsynhDispatcher();
	~WinAsynhDispatcher() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual void bind(SAsynchChannel channel);
	virtual CopletitionEvent nextEvent();
private:
	::HANDLE port_;
	std::vector<SWinAsynchChannel> keys_;
};


} // namespace io


#endif // __WIN_ASSYNCH_CHANNEL_HPP_INCLUDED__
