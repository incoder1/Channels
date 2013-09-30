/**
 * Windows implementation of file and file channel
 */

#include "file.hpp"

namespace io {

File::File(const char* path) BOOST_NOEXCEPT:
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
	DWORD result;
	if(!::ReadFile(id_,
	               reinterpret_cast<void*>(buffer.last().ptr()),
	               buffer.capacity(),
	              &result,
	               NULL)) {
		throw io_exception("Read file error");
	}
	buffer.move(result);
	return result;
}

size_t FileChannel::write(const byte_buffer& buffer) throw(io_exception)
{
	DWORD result;
	if(!::WriteFile(id_,
	                reinterpret_cast<void*>(buffer.position().ptr()),
	                buffer.length(),
	                &result,
	                NULL)
	  ) {
		throw io_exception("Write file error");
	}
	return result;
}


void FileChannel::seek(size_t offset, ReadWriteChannel::MoveMethod method) throw(io_exception)
{
	if(INVALID_SET_FILE_POINTER == ::SetFilePointer(id_,offset,NULL,method)) {
		throw io_exception("Can not move file pointer");
	}
}


FileChannel::~FileChannel()
{
	CloseHandle(id_);
}

} // namespace io

