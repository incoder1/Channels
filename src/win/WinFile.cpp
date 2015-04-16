/**
 * Windows implementation of file and file channel
 */
#include "prchdrs.h"
#include "helpers.hpp"
#include "WinFile.hpp"

namespace io {


inline void validate_file_handle(HANDLE hFile) {
	validate_io(INVALID_HANDLE_VALUE != hFile, "Can not open file. Reason:");
}

// File
File::File(const char* path) BOOST_NOEXCEPT_OR_NOTHROW:
	path_(path)
{}

bool File::create() const BOOST_NOEXCEPT_OR_NOTHROW
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
	bool result = INVALID_HANDLE_VALUE != hFile;
	if(result) {
		CloseHandle(hFile);
	}
	return result;
}

bool File::remove() const BOOST_NOEXCEPT_OR_NOTHROW
{
	bool result = exist();
	if(result) {
		result = ::DeleteFile(path_);
	}
	return result;
}

bool File::exist() const BOOST_NOEXCEPT_OR_NOTHROW
{
	::WIN32_FIND_DATA findFileData;
	::HANDLE handle = ::FindFirstFile(path_, &findFileData) ;
	bool result = handle != INVALID_HANDLE_VALUE;
	if(result) {
		::FindClose(handle);
	}
	return result;
}

SReadChannel File::openForRead()
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	validate_file_handle(hFile);
	return SReadChannel(new WinChannel(hFile, GENERIC_READ, true));
}

SWriteChannel  File::openForWrite()
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	validate_file_handle(hFile);
	return SWriteChannel(new WinChannel(hFile, GENERIC_WRITE, true));
}

SRandomAccessChannel File::openForReadWrite()
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_READ | GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
	validate_file_handle(hFile);
	return SRandomAccessChannel(new WinChannel(hFile, GENERIC_READ | GENERIC_WRITE, true));
}


// WinChannel
WinChannel::WinChannel(HANDLE id, DWORD desiredAccess, bool close) BOOST_NOEXCEPT_OR_NOTHROW:
	RandomAccessChannel(),
	object(),
	id_(id),
	desiredAccess_(desiredAccess),
	close_(close)
{
}

WinChannel::~WinChannel()
{
	if(close_) {
		::CloseHandle(id_);
	}
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
	DWORD result = 0;
	BOOL succeeded = ::WriteFile(id_,
	                vpos(buffer),
	                buffer.length(),
	                &result,
	                NULL);
	validate_io(succeeded,"Write file error.");
	return result;
}

inline uint64_t WinChannel::seek(int64_t offset, DWORD whence)
{
	return file_seek(id_,offset,whence);
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

} // namespace io

