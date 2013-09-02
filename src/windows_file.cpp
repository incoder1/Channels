/**
 * Windows implementation of file and file channel
 */

#include "file.hpp"

namespace io {

File::File(const char* path):
	path_(path)
{}

bool File::create()
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
	bool result = INVALID_HANDLE_VALUE != hFile;
	if(result) {
		CloseHandle(hFile);
	}
	return result;
}

/**
 * Deletes file
 */
bool File::errace()
{
	bool result = exist();
	if(result) {
		result = DeleteFile(path_);
	}
	return result;
}

bool File::exist() const
{
	WIN32_FIND_DATA findFileData;
	HANDLE handle = FindFirstFile(path_, &findFileData) ;
	bool result = handle != INVALID_HANDLE_VALUE;
	if(result) {
		FindClose(handle);
	}
	return result;
}

PReadChannel File::openForRead() throw(io_exception)
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(INVALID_HANDLE_VALUE == hFile) {
		throw io_exception("Can not open file");
	}
	return PReadChannel(new FileChannel(hFile));
}

PWriteChannel  File::openForWrite() throw(io_exception)
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(INVALID_HANDLE_VALUE == hFile) {
		throw io_exception("Can not open file");
	}
	return PWriteChannel(new FileChannel(hFile));
}

PReadWriteChannel  File::openForReadWrite() throw(io_exception)
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_READ | GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(INVALID_HANDLE_VALUE == hFile) {
		throw io_exception("Can not open file");
	}
	return PReadWriteChannel(new FileChannel(hFile));
}

// FileChannel
FileChannel::FileChannel(HANDLE id):
		ReadWriteChannel(),
		id_(id)
{}

size_t FileChannel::read(byte_buffer& buffer) throw(io_exception)
{
	size_t result = 0;
	if(!::ReadFile(id_,
	               reinterpret_cast<void*>(buffer.last().ptr()),
	               buffer.capacity(),
	               reinterpret_cast<PDWORD>(&result),
	               NULL)) {
		throw io_exception("Read file error");
	}
	buffer.move(result);
	return result;
}

size_t FileChannel::write(const byte_buffer& buffer) throw(io_exception)
{
	size_t result;
	if(!::WriteFile(id_,
	                reinterpret_cast<void*>(buffer.begin().ptr()),
	                buffer.length(),
	                reinterpret_cast<PDWORD>(&result),
	                NULL)
	  ) {
		throw io_exception("Write file error");
	}
	return result;
}

FileChannel::~FileChannel()
{
	CloseHandle(id_);
}

} // namespace io

