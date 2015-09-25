#ifndef CHANNELS_UNIXFILE_HPP_INCLUDED
#define CHANNELS_UNIXFILE_HPP_INCLUDED

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "abstractfile.hpp"
#include "unixasynchronousdevicechannel.hpp"

namespace io {

/**
 * ! \brief Unix depended blocking file Channel implementation.
 */
class CHANNEL_PUBLIC FileChannel:public RandomAccessChannel {
public:
	FileChannel(int file);
	virtual ~FileChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t read(byte_buffer& buffer) ;
	virtual std::size_t write(const byte_buffer& buffer) ;
	virtual std::size_t position();
	virtual std::size_t forward(uint64_t offset) ;
	virtual std::size_t backward(uint64_t offset) ;
	virtual std::size_t fromBegin(uint64_t offset) ;
	virtual std::size_t fromEnd(uint64_t offset) ;
private:
	std::size_t seek(int64_t offset, int whence);
private:
	int file_;
};

} // namespace io

#endif // CHANNELS_UNIXFILE_HPP_INCLUDED
