#ifndef WINDOWSFILE_HPP_INCLUDED
#define WINDOWSFILE_HPP_INCLUDED

#include <boost/make_shared.hpp>

#include "abstractfile.hpp"
#include "smallobject.hpp"

namespace io {

/**
 * ! \brief Windows depended blocking file Channel implementation.
 */
class CHANNEL_PUBLIC FileChannel:public virtual RandomAccessChannel,public virtual object {
private:
	inline uint64_t seek(uint64_t,DWORD method);
public:
	FileChannel(HANDLE id, DWORD desiredAccess, bool close) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~FileChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t read(byte_buffer& buffer) ;
	virtual std::size_t write(byte_buffer& buffer) ;
	virtual std::size_t position();
	virtual std::size_t forward(std::size_t offset) ;
	virtual std::size_t backward(std::size_t offset) ;
	virtual std::size_t fromBegin(std::size_t offset) ;
	virtual std::size_t fromEnd(std::size_t offset) ;
private:
	HANDLE id_;
	DWORD desiredAccess_;
	const bool close_;
};

} // namespace io

#endif // WINDOWSFILE_HPP_INCLUDED
