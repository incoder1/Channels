#ifndef ABSTRACTFILE_HPP_INCLUDED
#define ABSTRACTFILE_HPP_INCLUDED

#include "channels.hpp"

namespace io {

const char* get_temp_dir();
const char* generate_temp_file_name(const char* prefix);

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
	 * \return reference smart pointer on reading file channel
	 * \throw io_exception in case of failure
	 */
	SReadChannel openForRead();
	/**
	 * Open blocking write file channel
	 * \return reference smart pointer on writing file channel
	 * \throw io_exception in case of failure
	 */
	SWriteChannel openForWrite();
	/**
	 * Open blocking read-write file channel
	 * \return reference smart pointer on read-write file channel
	 * \throw io_exception in case of failure
	 */
	SRandomAccessChannel openForReadWrite();
private:
	const char* path_;
};

} // namespace io

#endif // ABSTRACTFILE_HPP_INCLUDED
