#include "file.hpp"

namespace io {

// File
File::File(const char* path) BOOST_NOEXCEPT_OR_NOTHROW:
path_(path)
{}

bool File::create() const
{
	::FILE *fd = ::open(path_, O_CREAT);
	bool result = NULL == fd;
	if(result) {
		::close(fd);
	}
	return result;
}

bool File::errace() const
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

SReadChannel File::openForRead() throw(io_exception)
{
	::FILE *fp = ::open(path_, O_RDONLY);
	if(NULL == *fp) {
		boost::throw_exception(io_exception("Can not open file"));
	}
	return SReadChannel(new FileChannel(fp));
}

SWriteChannel  File::openForWrite() throw(io_exception)
{
	::FILE *fp = ::open(path_, O_WONLY | O_APPEND);
	if(NULL == *fp) {
		boost::throw_exception(io_exception("Can not open file"));
	}
	return SWriteChannel(new FileChannel(hFile));
}

PReadWriteChannel  File::openForReadWrite() throw(io_exception)
{
	::FILE *fp = ::open(path_, O_RDWR | O_APPEND);
	if(NULL == *fp) {
		boost::throw_exception(io_exception("Can not open file"));
	}
	return PReadWriteChannel(new FileChannel(hFile));
}

// File Channel
FileChannel::FileChannel(::FILE* file) BOOST_NOEXCEPT_OR_NOTHROW:
	ReadWriteChannel(),
	file_(file, boost::bind(::close))
{}

~FileChannel::FileChannel BOOST_NOEXCEPT_OR_NOTHROW
{
}

std::size_t FileChannel::read(byte_buffer& buffer) throw(io_exception)
{
	std::size_t result = ::read(
	                    file_.get(),
	                    reinterpret_cast<void*>(&buffer.position()),
	                    buffer.capacity()
	                );
	buffer.move(result);
	return result;
}

std::size_t FileChannel::write(const byte_buffer& buffer) throw(io_exception)
{
	std::size_t result = ::write(
	                    file_.get(),
	                    reinterpret_cast<void*>(&buffer.position()),
	                    buffer.length(),
	                );
	return result;
}

} // namespace io
