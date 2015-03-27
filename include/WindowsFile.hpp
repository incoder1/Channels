#ifndef WINDOWSFILE_HPP_INCLUDED
#define WINDOWSFILE_HPP_INCLUDED

#include <boost/make_shared.hpp>

#include "abstractfile.hpp"
#include "smallobject.hpp"

namespace io {

/**
 * ! \brief Windows depended blocking file Channel implementation.
 */
class CHANNEL_PUBLIC FileChannel:public virtual object, public virtual ReadWriteChannel {
private:
	inline uint64_t seek(uint64_t,DWORD method) throw(io_exception);
public:
	FileChannel(HANDLE id, DWORD desiredAccess, bool close) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~FileChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t read(byte_buffer& buffer) throw(io_exception);
	virtual std::size_t write(byte_buffer& buffer) throw(io_exception);
	virtual std::size_t position();
	virtual std::size_t forward(std::size_t offset) throw (io_exception);
	virtual std::size_t backward(std::size_t offset) throw (io_exception);
	virtual std::size_t fromBegin(std::size_t offset) throw (io_exception);
	virtual std::size_t fromEnd(std::size_t offset) throw (io_exception);
private:
	HANDLE id_;
	DWORD desiredAccess_;
	const bool close_;
};

// AssychReadFileChannel
class CHANNEL_PUBLIC AssychReadFileChannel:public AsynchReadChannel
{
public:
	AssychReadFileChannel(HANDLE hFile,const read_callback& callback,std::size_t buffSize) throw (std::bad_alloc);
	virtual void read();
private:
	static VOID CALLBACK completionReadRoutine(DWORD errorCode, DWORD transfered, LPOVERLAPPED loverlapped);
private:
	HANDLE id_;
	OVERLAPPED overlaped_;
	byte_buffer buffer_;
};

} // namespace io

#endif // WINDOWSFILE_HPP_INCLUDED
