#include "file.hpp"

namespace io {

// File
File::File(const char* path) BOOST_NOEXCEPT:
	path_(path)
{}

bool File::create()
{
	::FILE *fd = ::open(path_, O_CREAT);
	bool result = NULL == fd;
	if(result) {
		::close(fd);
	}
	return result;
}

bool File::errace()
{
	bool result = exist();
	if(result) {
		::remove(path_);
	}
	return result;
}

bool File::exist() const
{
	struct ::stat  buffer;
	int res = ::stat(path_, &buffer);
	// Check file INODE exist
	return buff.st_ino != NULL;
}

PReadChannel File::openForRead() throw(io_exception)
{
	::FILE *fp = ::open(path_, O_RDONLY);
	if(NULL == *fp) {
		throw io_exception("Can not open file");
	}
	return PReadChannel(new FileChannel(fp));
}

PWriteChannel  File::openForWrite() throw(io_exception)
{
	::FILE *fp = ::open(path_, O_WONLY | O_APPEND);
	if(NULL == *fp) {
		throw io_exception("Can not open file");
	}
	return PWriteChannel(new FileChannel(hFile));
}

PReadWriteChannel  File::openForReadWrite() throw(io_exception)
{
	::FILE *fp = ::open(path_, O_RDWR | O_APPEND);
	if(NULL == *fp) {
		throw io_exception("Can not open file");
	}
	return PReadWriteChannel(new FileChannel(hFile));
}

// File Channel
FileChannel::FileChannel(::FILE* file) BOOST_NOEXCEPT:
	ReadWriteChannel(),
	file_(file)
{}

size_t FileChannel::read(byte_buffer& buffer) throw(io_exception)
{
	size_t result = ::read(
		file_
		reinterpret_cast<void*>(buffer.position().ptr()),
		buffer.capacity()
		);
	buffer.move(result);
	return result;
}

size_t FileChannel::write(const byte_buffer& buffer) throw(io_exception)
{
	size_t result = ::write(
		file_,
		reinterpret_cast<void*>(buffer.position().ptr()),
		buffer.length(),
		);
	return result;
}

} // namespace io
