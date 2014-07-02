/**
 * Windows implementation of file and file channel
 */
#include "prchdrs.h"
#include "WindowsFile.hpp"
#include <boost/pool/object_pool.hpp>
#include <boost/thread/once.hpp>
#include <cassert>
#include <cstdlib>

namespace io {

// helpers
inline void* vpos(const byte_buffer& buff) {
	return static_cast<void*>(buff.position().ptr());
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
	return SReadChannel(new FileChannel(hFile, true));
}

SWriteChannel  File::openForWrite() throw(io_exception)
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	validate_file_handle(hFile);
	return SWriteChannel(new FileChannel(hFile, true));
}

SReadWriteChannel  File::openForReadWrite() throw(io_exception)
{
	HANDLE hFile = ::CreateFile(path_, GENERIC_READ | GENERIC_WRITE , 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
	validate_file_handle(hFile);
	return SReadWriteChannel(new FileChannel(hFile, true));
}

// small object memory allocation
namespace _private_mmt {
	class Allocator:private boost::noncopyable {
	private:
		static Allocator* _instance;
		static boost::once_flag _once;
		static void initilizeSigleton() {
			_instance = new Allocator();
			std::atexit(&Allocator::utilizeSigleton);
		}
		static void utilizeSigleton(void) {
			delete _instance;
		}
	public:
		static Allocator* const instance() {
			boost::call_once(&Allocator::initilizeSigleton,_once);
			return _instance;
		}
		void* allocate() throw(std::bad_alloc) {
			return pool_.malloc();
		}
		void free(void *ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
			assert(size == sizeof(io::FileChannel));
			pool_.free(static_cast<io::FileChannel*>(ptr));
		}
	private:
		boost::object_pool<io::FileChannel> pool_;
	};

	Allocator* Allocator::_instance = NULL;

	boost::once_flag Allocator::_once = BOOST_ONCE_INIT;
}

void* FileChannel::operator new(std::size_t size) throw(std::bad_alloc)
{
	return _private_mmt::Allocator::instance()->allocate();
}

void FileChannel::operator delete(void *ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW
{
	_private_mmt::Allocator::instance()->free(ptr,size);
}

// FileChannel
FileChannel::FileChannel(HANDLE id, bool close) BOOST_NOEXCEPT_OR_NOTHROW:
	ReadWriteChannel(),
	id_(id),
	close_(close)
{
}

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


struct OVERLAPPEDEX:public ::OVERLAPPED {
public:
	OVERLAPPEDEX(const byte_buffer& buff, SAsynchCompletionHandler handler) BOOST_NOEXCEPT_OR_NOTHROW:
		::OVERLAPPED(),
		handler_(handler),
		buffer_(buff)
	{
		Internal = InternalHigh = 0,
		Offset = OffsetHigh = 0;
		Pointer = NULL;
		hEvent = NULL;
	}
	inline SAsynchCompletionHandler handler() BOOST_NOEXCEPT_OR_NOTHROW {
		return handler_;
	}
	inline byte_buffer buffer() BOOST_NOEXCEPT_OR_NOTHROW {
		return buffer_;
	}
private:
	SAsynchCompletionHandler handler_;
	byte_buffer buffer_;
};

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


void FileChannel::seek(std::size_t offset, MoveMethod method) throw(io_exception)
{
	if(INVALID_SET_FILE_POINTER == ::SetFilePointer(id_,offset,NULL,method)) {
		boost::throw_exception(io_exception("Can not move file pointer"));
	}
}

FileChannel::~FileChannel()
{
	if(close_) {
		::CloseHandle(id_);
	}
}

} // namespace io

