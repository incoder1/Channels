#ifndef __IO_WIN_ASYNCH_FILE_CHANNEL_HPP_INCLUDED__
#define __IO_WIN_ASYNCH_FILE_CHANNEL_HPP_INCLUDED__

#include <Windows.h>

#include <boost/scoped_ptr.hpp>
#include <boost/atomic/atomic.hpp>

#include "channels.hpp"
#include "smallobject.hpp"

namespace io {


class WinAsychFileChannel;
class CHANNEL_PUBLIC WinAsyncResult:public virtual AsyncResult, public virtual object {
public:
	WinAsyncResult(::HANDLE hFile,::LPOVERLAPPED overlapped,const completion_routine_f& routine);
	~WinAsyncResult() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t await() = 0;
	virtual bool active();
	virtual bool cancel() = 0;
	static VOID CALLBACK done_routine(DWORD errCode,DWORD transfered, LPOVERLAPPED overlapped);
private:
	inline void onComplete(int errorCode,std::size_t transfered);
private:
	::HANDLE hFile_;
	::LPOVERLAPPED overlapped_;
	completion_routine routine_;
	boost::atomics::atomic_bool active_;
};

class CHANNEL_PUBLIC WinAsychFileChannel:public virtual AsynchReadWriteChannel,public virtual object {
public:
	explicit WinAsychFileChannel(::HANDLE hFile);
	virtual SAsyncResult read(uint64_t pos,const completion_routine_f& callback);
	virtual SAsyncResult write(uint64_t pos,const byte_buffer& buffer);
	virtual SAsyncResult write(uint64_t pos,const byte_buffer& buffer,const completion_routine_f& callback);
	uint64_t position();
	~WinAsychFileChannel() BOOST_NOEXCEPT_OR_NOTHROW;
private:
	::HANDLE hFile_;
	::OVERLAPPED position_;
	boost::scoped_ptr<counter> counter_;
};


} // namesapce io

#endif // __IO_WIN_ASYNCH_FILE_CHANNEL_HPP_INCLUDED__
