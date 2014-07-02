#include "prchdrs.h"
#include "unixfile.hpp"

namespace io {

inline void validate_io(ssize_t res, const char* msg) {
	if(-1 == res) {
		boost::system::error_code ec;
		std::string smsg(msg);
		smgs.append("");
		smgs.append(ec.category().what());
		boost::throw_exception(io_exception(smsg));
	}
}

FileChannel::FileChannel(int file) BOOST_NOEXCEPT_OR_NOTHROW:
	file_(file)
{}

FileChannel::~FileChannel() BOOST_NOEXCEPT_OR_NOTHROW
{
	::close(file_);
}

std::size_t FileChannel::read(byte_buffer& buffer) throw(io_exception)
{
	::ssize_t result = ::read(file_, vpos(buffer), buffer.capacity());
	validate_io(result, "Read file error");
	buffer.move((std::size_t)result);
	return static_cast<std::size_t>(result);
}

std::size_t FileChannel::write(const byte_buffer& buffer) throw(io_exception)
{
	ssize_t result = ::write(file_, vpos(buffer), buffer.length());
	validate_io(result, "Write file error");
	return result;
}

void FileChannel::seek(std::size_t offset, MoveMethod method) throw(io_exception)
{
	off_t res = lseek(file_, offset, method);
	validate_io(static_cast<ssize_t>(result), "Move file pointer error");
	return result;
}

void * FileChannel::operator new(std::size_t size) throw(std::bad_alloc)
{
}

void FileChannel::operator delete(void* ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW
{
}

} // namespace io
