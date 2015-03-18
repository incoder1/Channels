/**
 * Windows implementation of file and file channel
 */
#include "prchdrs.h"

#include "WindowsFile.hpp"

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

bool File::errace() const BOOST_NOEXCEPT_OR_NOTHROW
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

SReadChannel File::openForRead() throw(io_exception)
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	validate_file_handle(hFile);
	return SReadChannel(new FileChannel(hFile, GENERIC_READ, true));
}

SWriteChannel  File::openForWrite() throw(io_exception)
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	validate_file_handle(hFile);
	return SWriteChannel(new FileChannel(hFile, GENERIC_WRITE, true));
}

SReadWriteChannel  File::openForReadWrite() throw(io_exception)
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_READ | GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
	validate_file_handle(hFile);
	return SReadWriteChannel(new FileChannel(hFile, GENERIC_READ | GENERIC_WRITE, true));
}


// FileChannel
FileChannel::FileChannel(HANDLE id, DWORD desiredAccess, bool close) BOOST_NOEXCEPT_OR_NOTHROW:
	ReadWriteChannel(),
	object(),
	id_(id),
	desiredAccess_(desiredAccess),
	close_(close)
{
}

FileChannel::~FileChannel()
{
	if(close_) {
		::CloseHandle(id_);
	}
}

std::size_t FileChannel::read(byte_buffer& buffer) throw(io_exception)
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

std::size_t FileChannel::write(byte_buffer& buffer) throw(io_exception)
{
	DWORD result = 0;
	if(!buffer.full()) {
		BOOL succeeded = ::WriteFile(id_,
	                vpos(buffer),
	                buffer.length(),
	                &result,
	                NULL);
		validate_io(succeeded,"Write file error.");
		buffer.move(result);
	}
	return result;
}

std::size_t FileChannel::seek(std::size_t offset, DWORD whence) throw(io_exception)
{
	::LARGE_INTEGER li;
	li.QuadPart = static_cast<LONG>(offset);
	validate_io(::SetFilePointerEx(id_, li, &li,whence), "Can not move file pointer");
	return static_cast<std::size_t>(li.QuadPart);
}

std::size_t FileChannel::position() {
	return seek(0, FILE_CURRENT);
}

std::size_t FileChannel::forward(std::size_t offset) throw (io_exception) {
	return seek(offset, FILE_CURRENT);
}

std::size_t FileChannel::backward(std::size_t offset) throw (io_exception)
{
	ssize_t step = static_cast<std::size_t>(offset);
	return seek(-step, FILE_CURRENT);
}

std::size_t FileChannel::fromBegin(std::size_t offset) throw (io_exception) {
	return seek(offset, FILE_BEGIN);
}

std::size_t FileChannel::fromEnd(std::size_t offset) throw (io_exception) {
	ssize_t step = static_cast<ssize_t>(offset);
	return seek(-step, FILE_END);
}

// AsychReadFileChannel

} // namespace io

