#ifndef __WIN_ASSYNCH_CHANNEL_HPP_INCLUDED__
#define __WIN_ASSYNCH_CHANNEL_HPP_INCLUDED__

#include "channels.hpp"

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/executors/basic_thread_pool.hpp>
#include <boost/thread/future.hpp>
#include <boost/thread/thread.hpp>
#include <boost/unordered_map.hpp>

#include <vector>
#include <windows.h>

namespace io {

class CHANNEL_PUBLIC WinAsynchChannel:public virtual AsynchChannel,public virtual object {
public:
	WinAsynchChannel(::HANDLE handle,const completion_handler_f& recvHnd,const completion_handler_f& sndHnd);
	~WinAsynchChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual void send(const byte_buffer& buffer,int64_t pos) const;
	virtual void receive(byte_buffer& buffer,int64_t pos) const;
	BOOST_FORCEINLINE ::HANDLE descriptor() const {
		return handle_;
	}
protected:
	friend class WinAsynhDispatcher;
private:
	::HANDLE handle_;
};

DECLARE_PTR_T(WinAsynchChannel);

class WinAsynhDispatcher:public virtual AsynhDispatcher,public virtual object
{
public:
	WinAsynhDispatcher(std::size_t maxThreads);
	~WinAsynhDispatcher() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual void bind(SAsynchChannel channel);
	virtual void start();
private:
	void routine();
private:
	std::size_t maxThreads_;
	::HANDLE port_;
	std::vector<SWinAsynchChannel> keys_;
	boost::thread_group pool_;
};


} // namespace io


#endif // __WIN_ASSYNCH_CHANNEL_HPP_INCLUDED__
