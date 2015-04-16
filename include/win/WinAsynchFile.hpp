#ifndef __IO_WIN_ASYNCH_FILE_CHANNEL_HPP_INCLUDED__
#define __IO_WIN_ASYNCH_FILE_CHANNEL_HPP_INCLUDED__

#include "winver.h"
#include <windows.h>

#include <boost/scoped_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/atomic/atomic.hpp>

#include "channels.hpp"
#include "smallobject.hpp"

namespace io {


class CHANNEL_PUBLIC WinAsyncResult:public virtual AsyncResult, public virtual object {
public:
	WinAsyncResult(::HANDLE hFile,uint64_t position,const byte_buffer& buffer);
	virtual ~WinAsyncResult() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t await();
	virtual bool active();
	virtual bool cancel();
	virtual byte_buffer buffer();
	inline ::LPOVERLAPPED overlapped() const {
		return overlapped_.get();
	}
private:
	::HANDLE hFile_;
	boost::scoped_ptr<::OVERLAPPED> overlapped_;
	const completion_routine_f routine_;
	boost::atomics::atomic_bool active_;
	byte_buffer buffer_;
};

// Never used for the disk files, only named pipes etc
class CHANNEL_PUBLIC WinAsychChannel:public virtual AsynchReadWriteChannel,public virtual object {
public:
	explicit WinAsychChannel(::HANDLE id,::HANDLE completionPort);
	~WinAsychChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual SAsyncResult read(uint64_t pos,std::size_t max);
	virtual SAsyncResult write(const byte_buffer& buffer);
private:
	inline uint64_t seek(int64_t offset, DWORD whence);
private:
	::HANDLE hFile_;
	::HANDLE cmpltPort_;
};

} // namesapce io

#endif // __IO_WIN_ASYNCH_FILE_CHANNEL_HPP_INCLUDED__
