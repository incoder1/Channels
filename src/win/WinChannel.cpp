#include "prchdrs.h"
#include "WinChannel.hpp"
#include "helpers.hpp"

namespace io {

// WinChannel
WinChannel::WinChannel(::HANDLE id, ::DWORD desiredAccess) BOOST_NOEXCEPT_OR_NOTHROW:
	RandomAccessChannel(),
	object(),
	id_(id),
	desiredAccess_(desiredAccess)
{}

WinChannel::~WinChannel()
{
	::FlushFileBuffers(id_);
	::CloseHandle(id_);
}

std::size_t WinChannel::read(byte_buffer& buffer)
{
	DWORD result;
	BOOL succeeded = ::ReadFile(id_,
	                          vpos(buffer),
	                          buffer.capacity(),
	                          &result,
	                          NULL);
	if(!succeeded && 0 != result ) {
		validate_io(succeeded,"Read file error.");
	}
	if(result > 0) {
		buffer.move(result-1);
	}
	return result;
}

std::size_t WinChannel::write(const byte_buffer& buffer)
{
	::DWORD result = 0;
	::BOOL succeeded = ::WriteFile(id_,
	                vpos(buffer),
	                buffer.length(),
	                &result,
	                NULL);
	validate_io(succeeded,"Write file error.");
	return result;
}

inline uint64_t WinChannel::seek(int64_t offset, ::DWORD whence)
{
  ::LARGE_INTEGER li;
  li.QuadPart = offset;
  validate_io(::SetFilePointerEx(id_, li, &li,whence), "Can not move file pointer");
  return (uint64_t)li.QuadPart;
}

uint64_t WinChannel::position() {
	return seek(0, FILE_CURRENT);
}

uint64_t WinChannel::forward(uint64_t offset) {
	return seek(offset, FILE_CURRENT);
}

uint64_t WinChannel::backward(uint64_t offset)
{
	return seek(-offset, FILE_CURRENT);
}

uint64_t WinChannel::fromBegin(uint64_t offset) {
	return seek(offset, FILE_BEGIN);
}

uint64_t WinChannel::fromEnd(uint64_t offset) {
	return seek(-offset, FILE_END);
}


} // namesapce io
