#ifndef WINDOWSFILE_HPP_INCLUDED
#define WINDOWSFILE_HPP_INCLUDED

#include "abstractfile.hpp"
#include "smallobjectpool.hpp"

namespace io {

/**
 * ! \brief Windows depended blocking file Channel implementation.
 */
class CHANNEL_PUBLIC FileChannel:public virtual ReadWriteChannel, public virtual SmallObject {
public:
	FileChannel(HANDLE id, bool close) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~FileChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t read(byte_buffer& buffer) throw(io_exception);
	virtual std::size_t write(const byte_buffer& buffer) throw(io_exception);
	virtual void seek(std::size_t offset, MoveMethod method) throw(io_exception);
private:
	HANDLE id_;
	const bool close_;
};

} // namespace io

#endif // WINDOWSFILE_HPP_INCLUDED
