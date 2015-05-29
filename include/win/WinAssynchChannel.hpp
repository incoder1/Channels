#ifndef __WIN_ASSYNCH_CHANNEL_HPP_INCLUDED__
#define __WIN_ASSYNCH_CHANNEL_HPP_INCLUDED__

#include "channels.hpp"

#include <list>

#include <boost/atomic.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>

#include <windows.h>

namespace io {

enum asynh_operation {
	ASYNCH_SEND,
	ASYNCH_RECEIVE
};

class AssynchChannel;

class CHANNEL_PUBLIC SelectionKey {
public:
	SelectionKey(const std::size_t transfered, const byte_buffer buff) {
		channel_(channel_),
		operation_(operation),
		transfered_(transfered),
		buff(buff)
	{}
	BOOST_FORCEINLINE const AssynchChannel* channel() {
		return channel_;
	}
	BOOST_FORCEINLINE asynh_operation operation() {
		return operation_;
	}
	BOOST_FORCEINLINE const std::size_t transfered() {
		return transfered_;
	}
	BOOST_FORCEINLINE byte_buffer buffer() {
		return buffer;
	}
private:
	const AssynchChannel* channel_;
	asynh_operation operation_;
	const std::size_t transfered_;
	byte_buffer buff_;
};

class CHANNEL_PUBLIC Selector:public object {
BOOST_MOVABLE_BUT_NOT_COPYABLE(Selector)
public:
	static boost::shared_ptr<Selector> create() {
		return boost::make_shared<Selector>();
	}
	~Selector() BOOST_NOEXCEPT_OR_NOTHROW;
	SelectionKey select();
private:
	Selector();
private:
	boost::mutex mutex_;
	::HANDLE ioCnPrt_;
};

DECLARE_PTR_T(Selector);

class AssynchChannel:public object {
protected:
	explicit AssynchChannel(SSelector selector);
	inline const Selector* selector() {
		return selector_.get();
	}
public:
	virtual void send(const byte_buffer& buff, uint64_t position) const = 0;
private:
	SSelector selector_;
};

DECLARE_PTR_T(AssynchChannel);

class WinAsynchChannel:public virtual AssynchChannel {
public:
	WinAsynchChannel(::HANDLE hDevice);
	virtual ~WinAsynchChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual void send(const byte_buffer& buff, uint64_t position) const;
public:
	::HANDLE hDevice_;
};


} // namespace io


#endif // __WIN_ASSYNCH_CHANNEL_HPP_INCLUDED__
