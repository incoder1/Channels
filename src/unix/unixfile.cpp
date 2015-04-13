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
	return -1 != ::creat(path_, O_CREAT);
}

bool File::remove() const BOOST_NOEXCEPT_OR_NOTHROW
{
	return -1 != ::remove(path_);
}

bool File::exist() const BOOST_NOEXCEPT_OR_NOTHROW
{
	struct ::stat st;
	return 0 == ::stat(path_,&st);
}

SReadChannel File::openForRead()
{
	int fd = ::open(path_, O_APPEND | O_SYNC, O_RDONLY);
	validate_io(fd, "Can not open file");
	return SReadChannel(new FileChannel(fd));
}

SWriteChannel File::openForWrite()
{
	int fd = ::open(path_, O_APPEND | O_SYNC, O_WRONLY);
	validate_io(fd, "Can not open file");
	return SWriteChannel(new FileChannel(fd));
}

SRandomAccessChannel File::openForReadWrite()
{
	int fd = ::open(path_, O_APPEND | O_SYNC, O_RDWR);
	validate_io(fd, "Can not open file");
	return SRandomAccessChannel(new FileChannel(fd));
}

// FileChannel
FileChannel::FileChannel(int file) BOOST_NOEXCEPT_OR_NOTHROW:
	file_(file)
{}

FileChannel::~FileChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
	assert(::close(file_));
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

static bool determ_endian() {
	short num = 1;
	return *(char *)&num == 1;
}

std::size_t FileChannel::seek(std::size_t offset, int whence)
{
	off_t res = ::lseek(file_, offset, whence);
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
