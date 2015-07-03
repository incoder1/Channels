#ifndef WINDOWSFILE_HPP_INCLUDED
#define WINDOWSFILE_HPP_INCLUDED

#include "winver.h"
#include <windows.h>

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
};

} // namespace io

#endif // WINDOWSFILE_HPP_INCLUDED
