#ifndef __WINCHANNEL_HPP_INCLUDED__
#define __WINCHANNEL_HPP_INCLUDED__

#include "channels.hpp"

namespace io {

/**
 * ! \brief Generic blocking channel implementation
 * Can work with in a system device provided by the HANDLE descriptor
 * such as: file, system streaming device (in/out/err etc), named and unnamed pipes so one.
 * \note Also can work with the sockets in blocking mode, but is not recommended because of winsocks2
 * backward compatibility issues.
 * See network.hpp instead.
 */
class CHANNEL_PUBLIC WinChannel:public virtual RandomAccessChannel {
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
private:
	::HANDLE id_;
	::DWORD desiredAccess_;
};


} // namespace io


#endif // __WINCHANNEL_HPP_INCLUDED__
