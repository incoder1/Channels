#include "prchdrs.h"
#include "unixfile.hpp"
#include "helpers.hpp"

namespace io {

// File
File::File(const char* path) BOOST_NOEXCEPT_OR_NOTHROW:
	path_(path)
{
}

bool File::create() const BOOST_NOEXCEPT_OR_NOTHROW
{
	int fd = ::creat(path_, S_IRUSR |S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH );
	bool result = fd != 0;
	if(result) {
        fd = ::close(fd);
	}
	return result;
}

bool File::remove() const BOOST_NOEXCEPT_OR_NOTHROW
{
	return ::remove(path_) >= 0;
}

bool File::exist() const BOOST_NOEXCEPT_OR_NOTHROW
{
	struct ::stat st;
	return 0 == ::stat(path_,&st);
}

SReadChannel File::openForRead()
{
	int fd = ::open(path_, O_RSYNC | O_LARGEFILE | O_RDONLY);
	validate_io(fd, "Can not open file");
	return SReadChannel(new FileChannel(fd));
}

SWriteChannel File::openForWrite()
{
	int fd = ::open(path_, O_APPEND | O_DSYNC | O_LARGEFILE | O_WRONLY);
	validate_io(fd, "Can not open file");
	return SWriteChannel(new FileChannel(fd));
}

SRandomAccessChannel File::openForReadWrite()
{
	int fd = ::open(path_, O_APPEND | O_SYNC | O_LARGEFILE, O_RDWR);
	validate_io(fd, "Can not open file");
	return SRandomAccessChannel(new FileChannel(fd));
}


SAsyncChannel File::openAsynchronous(boost::asio::io_service& ios) {
    int fd = ::open(path_, O_DIRECT | O_NONBLOCK, O_RDWR);
    validate_io(fd, "Can not open file for asynchronous io");
	return SAsyncChannel(new UnixAsynchronousDeviceChannel(ios, fd));
}

// FileChannel
FileChannel::FileChannel(int file):
	file_(file)
{
    assert(file);
}

FileChannel::~FileChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
	assert(0 == ::close(file_));
}

std::size_t FileChannel::read(byte_buffer& buffer)
{
	::ssize_t result = ::read(file_, vpos(buffer), buffer.capacity());
	validate_io(result, "Read file error");
	buffer.move(static_cast<std::size_t>(result));
	return static_cast<std::size_t>(result);
}

std::size_t FileChannel::write(const byte_buffer& buffer)
{
	ssize_t result = ::write(file_, vpos(buffer), buffer.length());
	validate_io(result, "Write file error");
	return result;
}

std::size_t FileChannel::seek(int64_t offset, int whence)
{
	off_t res = ::lseek64(file_, offset, whence);
	validate_io(static_cast<ssize_t>(res) > 0, "Move file pointer error");
	return static_cast<std::size_t>(res);
}

std::size_t FileChannel::position() {
	return seek(0,SEEK_CUR);
}

std::size_t FileChannel::forward(uint64_t offset)
{
	return seek(offset,SEEK_CUR);
}

std::size_t FileChannel::backward(uint64_t offset)
{
	return seek(-((off_t)offset),SEEK_CUR);
}

std::size_t FileChannel::fromBegin(uint64_t offset)  {
	return seek(offset,SEEK_SET);
}

std::size_t FileChannel::fromEnd(uint64_t offset)  {
	return seek(-((off_t)offset),SEEK_END);
}


} // namespace io
