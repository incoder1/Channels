#ifndef CHANNELS_UNIXFILE_HPP_INCLUDED
#define CHANNELS_UNIXFILE_HPP_INCLUDED

#include "abstractfile.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace io {

/**
 * ! \brief Unix depended blocking file Channel implementation.
 */
class CHANNEL_PUBLIC FileChannel:public virtual ReadWriteChannel {
public:
	FileChannel(int file) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~FileChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t read(byte_buffer& buffer) throw(io_exception);
	virtual std::size_t write(const byte_buffer& buffer) throw(io_exception);
	virtual void seek(std::size_t offset, MoveMethod method) throw(io_exception);
	static void * operator new(std::size_t size) throw(std::bad_alloc);
	static void operator delete(void* ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW;
private:
	int file_;
};

} // namespace io

#endif // CHANNELS_UNIXFILE_HPP_INCLUDED
