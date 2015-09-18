/**
 * Windows implementation of file and file channel
 */
#include "prchdrs.h"
#include "WinFile.hpp"
#include "helpers.hpp"

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
	::HANDLE hFile = ::CreateFile(path_, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	validate_file_handle(hFile);
	return SReadChannel(new WinChannel(hFile, GENERIC_READ));
}

SWriteChannel  File::openForWrite()
{
	::HANDLE hFile = ::CreateFile(path_, GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	validate_file_handle(hFile);
	return SWriteChannel(new WinChannel(hFile, GENERIC_WRITE));
}

SRandomAccessChannel File::openForReadWrite()
{
	::HANDLE hFile = ::CreateFile(path_, GENERIC_READ | GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	validate_file_handle(hFile);
	return SRandomAccessChannel(new WinChannel(hFile, GENERIC_READ | GENERIC_WRITE));
}

SAsynchronousChannel File::openAsynchronous(boost::asio::io_service& ios) {
	::HANDLE hFile = ::CreateFile(path_, GENERIC_READ | GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
	validate_file_handle(hFile);
	return SAsynchronousChannel(new WinAsynchronousHandleChannel(ios,hFile));
}


} // namespace io

