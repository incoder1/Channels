#ifndef FILE_HPP_INCLUDED
#define FILE_HPP_INCLUDED

#include "channels_config.h"
#include "channels.hpp"

#ifdef PLATFORM_UNIX
# 	include <unistd.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#endif // PLATFORM_UNIX


namespace io  {

/**
 * ! \brief Abstraction for representing file. File system only, no sockets, devices etc.
*/
class CHANNEL_PUBLIC File {
private:
	const char* path_;
public:
	/**
	 * Creates new file, using path (file name)
	 * \param path path to file (file name) Path is operating system specific
	 * \note ANSI/ASCII 8-bytes peer character paths (file name) only supported in this version
	 */
	explicit File(const char* path) BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	 * Creates a new file
	 * \return whether successful
	 */
	bool create() const;
	/**
	 * Deletes file from dist
	 * \return whether successful
	 */
	bool errace() const;
	/**
	 * Returns file exist sate
	 * \return whether file with this path exist
	 */
	bool exist() const;
	/**
	 * Open blocking read file channel
	 * \return reference smart pointer on reading file channel
	 */
	PReadChannel openForRead() throw(io_exception);
	/**
	 * Open blocking write file channel
	 * \return reference smart pointer on writing file channel
	 */
	PWriteChannel openForWrite() throw(io_exception);
	/**
	 * Open blocking read-write file channel
	 * \return reference smart pointer on read-write file channel
	 */
	PReadWriteChannel openForReadWrite() throw(io_exception);
};


#ifdef PLATFROM_WINDOWS
/**
 * ! \brief Windows depended blocking file Channel implementation.
 */
class CHANNEL_PUBLIC FileChannel:public ReadWriteChannel {
private:
	HANDLE id_;
public:
	explicit FileChannel(HANDLE id) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~FileChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t read(byte_buffer& buffer) throw(io_exception);
	virtual std::size_t write(const byte_buffer& buffer) throw(io_exception);
	virtual void seek(std::size_t offset, ReadWriteChannel::MoveMethod method) throw(io_exception);
};

#endif


#ifdef PLATFORM_UNIX
/**
 * ! \brief Unix depended file channel implementation
 */
class FileChannel:public ReadWriteChannel {
private:
	boost::shared_ptr<::FILE> file_;
public:
	explicit FileChannel(::FILE* file) BOOST_NOEXCEPT_OR_NOTHROW;
	~FileChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t read(byte_buffer& buffer) throw(io_exception);
	virtual std::size_t write(const byte_buffer& buffer) throw(io_exception);
};
#endif // PLATFORM_UNIX


} // namespace io


#endif // FILE_HPP_INCLUDED
