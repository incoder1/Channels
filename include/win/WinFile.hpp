#ifndef WINDOWSFILE_HPP_INCLUDED
#define WINDOWSFILE_HPP_INCLUDED

#include "winver.h"
#include <windows.h>

#include <boost/scoped_ptr.hpp>

#include "abstractfile.hpp"
#include "smallobject.hpp"

namespace io {

/**
 * ! \brief Windows system handle blocking channel implementation.
 */
class CHANNEL_PUBLIC WinChannel:public virtual RandomAccessChannel,public virtual object {
private:
	inline uint64_t seek(int64_t offset, DWORD whence);
public:
	WinChannel(HANDLE id, DWORD desiredAccess) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~WinChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t read(byte_buffer& buffer);
	virtual std::size_t write(const byte_buffer& buffer);
	virtual uint64_t position();
	virtual uint64_t forward(uint64_t offset);
	virtual uint64_t backward(uint64_t offset);
	virtual uint64_t fromBegin(uint64_t offset);
	virtual uint64_t fromEnd(uint64_t offset);
	inline ::HANDLE handle() { return id_;}
private:
	::HANDLE id_;
	::DWORD desiredAccess_;
	boost::scoped_ptr<::OVERLAPPED> overlapped_;
};

inline const char* get_temp_dir()
{
	static char result[MAX_PATH+1];
	::GetTempPathA(MAX_PATH+1,result);
	return result;
}

inline const char* generate_temp_file_name(const char* prefix) {
	static char result[MAX_PATH+1];
	::GetTempFileNameA(get_temp_dir(),prefix,0,result);
	return result;
}

} // namespace io

#endif // WINDOWSFILE_HPP_INCLUDED
