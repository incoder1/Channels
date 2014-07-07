#ifndef ABSTRACTFILE_HPP_INCLUDED
#define ABSTRACTFILE_HPP_INCLUDED

#include "channels.hpp"

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
	bool create() const BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	 * Deletes file from dist
	 * \return whether successful
	 */
	bool errace() const BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	 * Returns file exist sate
	 * \return whether file with this path exist
	 */
	bool exist() const BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	 * Open blocking read file channel
	 * \return reference smart pointer on reading file channel
	 */
	SReadChannel openForRead() throw(io_exception);
	/**
	 * Open blocking write file channel
	 * \return reference smart pointer on writing file channel
	 */
	SWriteChannel openForWrite() throw(io_exception);
	/**
	 * Open blocking read-write file channel
	 * \return reference smart pointer on read-write file channel
	 */
	SReadWriteChannel openForReadWrite() throw(io_exception);
};

} // namespace io

#endif // ABSTRACTFILE_HPP_INCLUDED
