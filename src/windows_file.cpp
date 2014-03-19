/**
 * Windows implementation of file and file channel
 */

#include "file.hpp"

namespace io {

// helpers
inline void* vpos(const byte_buffer& buff) {
	return static_cast<void*>(&buff.position());
}

static std::string last_error_str(DWORD lastError)
{
	std::string result;
	LPVOID lpMsgBuf;
	static DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD bufLen = ::FormatMessage(flags,NULL,lastError,
	                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	                   (LPTSTR) &lpMsgBuf, 0, NULL );
	if (bufLen) {
		LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
		result.append(lpMsgStr, lpMsgStr+bufLen);
		::LocalFree(lpMsgBuf);
	}
	return result;
}

inline void validate_io(BOOL ioResult, const char* message) throw(io_exception) {
	if(!ioResult) {
		DWORD lastError = ::GetLastError();
		if(ERROR_IO_PENDING != lastError) {
			std::string errMsg(message);
			errMsg.append(" ");
			errMsg.append(last_error_str(lastError));
			boost::throw_exception(io_exception(errMsg));
		}
	}
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
		result = DeleteFile(path_);
	}
	return result;
}

bool File::exist() const BOOST_NOEXCEPT_OR_NOTHROW
{
	WIN32_FIND_DATA findFileData;
	HANDLE handle = ::FindFirstFile(path_, &findFileData) ;
	bool result = handle != INVALID_HANDLE_VALUE;
	if(result) {
		FindClose(handle);
	}
	return result;
}

SReadChannel File::openForRead() throw(io_exception)
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(INVALID_HANDLE_VALUE == hFile) {
		boost::throw_exception(io_exception("Can not open file"));
	}
	return SReadChannel(new FileChannel(hFile));
}

SWriteChannel  File::openForWrite() throw(io_exception)
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(INVALID_HANDLE_VALUE == hFile) {
		boost::throw_exception(io_exception("Can not open file"));
	}
	return SWriteChannel(new FileChannel(hFile));
}

PReadWriteChannel  File::openForReadWrite() throw(io_exception)
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_READ | GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(INVALID_HANDLE_VALUE == hFile) {
		boost::throw_exception(io_exception("Can not open file"));
	}
	return PReadWriteChannel(new FileChannel(hFile));
}

// FileChannel
FileChannel::FileChannel(HANDLE id) BOOST_NOEXCEPT_OR_NOTHROW:
	ReadWriteChannel(),
	id_(id)
{}

std::size_t FileChannel::read(byte_buffer& buffer) throw(io_exception)
{
	DWORD result;
	BOOL succeeded = ::ReadFile(id_,
	                          vpos(buffer),
	                          buffer.capacity(),
	                          &result,NULL);
	validate_io(succeeded,"Read file error.");
	buffer.move(result);
	return result;
}

std::size_t FileChannel::write(const byte_buffer& buffer) throw(io_exception)
{
	DWORD result;
	BOOL succeeded = ::WriteFile(id_,
	                vpos(buffer),
	                buffer.length(),
	                &result,
	                NULL);
	validate_io(succeeded,"Write file error.");
	return result;
}


void FileChannel::seek(std::size_t offset, ReadWriteChannel::MoveMethod method) throw(io_exception)
{
	if(INVALID_SET_FILE_POINTER == ::SetFilePointer(id_,offset,NULL,method)) {
		boost::throw_exception(io_exception("Can not move file pointer"));
	}
}


FileChannel::~FileChannel()
{
	::CloseHandle(id_);
}

} // namespace io

