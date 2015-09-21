#ifndef ABSTRACTFILE_HPP_INCLUDED
#define ABSTRACTFILE_HPP_INCLUDED

#include "channels.hpp"
#include "asynchronous.hpp"

namespace io {

/**
 * ! \brief Abstraction for representing file. File system only, no sockets, devices etc.
*/
class CHANNEL_PUBLIC File {
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
	 * Deletes file
	 * \return whether successful
	 */
	bool remove() const BOOST_NOEXCEPT_OR_NOTHROW;

	/**
	 * Returns file exist sate
	 * \return whether file with this path exist
	 */
	bool exist() const BOOST_NOEXCEPT_OR_NOTHROW;

	/**
	 * Open blocking read file channel
	 * \return smart pointer on reading file channel
	 * \throw io_exception in case of failure
	 */
	SReadChannel openForRead();

	/**
	 * Open blocking write file channel
	 * \return smart pointer on writing file channel
	 * \throw io_exception in case of failure
	 */
	SWriteChannel openForWrite();

	/**
	 * Open blocking mode file channel for the read/write operations with random access
	 * \return smart pointer on random access file channel
	 */
	SRandomAccessChannel openForReadWrite();

	/**
	 * Open file for the asynchronous non-blocking full access
	 * \param ios reference on constructed boost asio io_service
	 * \return smart pointer asynchronous channel
	 */
	SAsyncChannel openAsynchronous(boost::asio::io_service& ios);

private:
	const char* path_;
};

} // namespace io

#endif // ABSTRACTFILE_HPP_INCLUDED
