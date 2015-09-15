#ifndef __IO_CHARSETS_HPP_INCLUDED__
#define __IO_CHARSETS_HPP_INCLUDED__

#include "channels_config.h"

#include <boost/atomic.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/unordered_map.hpp>

namespace io {

/**
 * ! \brief Represent basic information about character set
 */
class CHANNEL_PUBLIC Charset {
BOOST_MOVABLE_BUT_NOT_COPYABLE(Charset)
public:
	/**
	 * Constructs character set
	 * \param id
	 *			the unique code page identifier (Win32 API identifier)
	 * \param name
	 *			the unique name of character set (libiconv/posix name)
	 *	\param charSize
	 *		the size in bytes of single character in this character set
	 *	\param isUnicode
	 *		whether current char set is Unicode code-page representation
	 */
	Charset(std::size_t id, const char* name, const std::size_t charSize, bool isUnicode) BOOST_NOEXCEPT_OR_NOTHROW;

	/**
	 * Constructs character set
	 * \param id
	 *			the unique code page identifier (Win API identifier)
	 * \param name
	 *			the unique name of character set (libiconv/posix name)
	 *	\param charSize
	 *		the size in bytes of single character in this character set
	 */
	Charset(std::size_t id, const char* name, const std::size_t charSize) BOOST_NOEXCEPT_OR_NOTHROW;

	/**
	 * Returns code page identifier (the value is the same as Win API code page)
	 * \return code page identifier
	 */
	std::size_t id() const BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	 * Returns unique name of character set
	 * \return unique name of character set
	 */
	const char* name() const BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	 * Returns maximal size in bytes of single character
	 * \return size in bytes of single character
	 */
	std::size_t  charSize() const BOOST_NOEXCEPT_OR_NOTHROW;

	/**
	 * Return whether current char set is Unicode code-page representation
	 * \return whether current char set is Unicode code-page representation
	 */
	bool isUnicode() const BOOST_NOEXCEPT_OR_NOTHROW;

	/**
	 * Compare this charset with a pointer to the another
	 * \param oth
	 *			constant pointer to the charset
	 * \return whether charsets are equal. If {@code oth} is {@code nullptr} return false
	 */
	bool equal(const Charset* oth) const BOOST_NOEXCEPT_OR_NOTHROW;

private:
	const std::size_t id_;
	const char* name_;
	const std::size_t charSize_;
	bool unicode_;
};

/**
 * ! \brief An helping factory which allows obtain character set reference by it name of system identifier
 */
class CHANNEL_PUBLIC Charsets:private boost::noncopyable {
private:
	typedef boost::unordered_map<std::string,const Charset*> ctmap_t;
	Charsets() BOOST_NOEXCEPT_OR_NOTHROW;
	const Charset* find(const char* name) const BOOST_NOEXCEPT_OR_NOTHROW;
	const Charset* find(std::size_t id) const BOOST_NOEXCEPT_OR_NOTHROW;
	static Charsets* instance();
	static void release() BOOST_NOEXCEPT_OR_NOTHROW;
	BOOST_FORCEINLINE void insert(const std::string& name, const Charset* ptr);
public:
	/**
	 * Obtains a reference on character set by it string name representation
	 * \param character set name
	 * \return reference on character set, or null pointer when character set is not supported
	 * \throw never throws
	 */
	static const Charset* forName(const char* name) BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	 * Obtains a reference on character set by it number identifier
	 * \param id
	 *			 character set identifier, the identifier is equal Microsoft windows code-page identifier.
	 *			 Anyway same ids are used for any implemented character set conversation engines.
	 * \return reference on character set, or null pointer when character set is not supported
	 * \throw never throws
	 */
	static const Charset* forId(std::size_t id) BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	 * Returns reference on character set describing UTF-16LE code page.
	 * This code page is default UNICODE representation in MS Windows
	 * \return reference on character set describing UTF-16LE code page
	 * \throw never throws
	 */
	static const Charset* utf16le() BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	* Returns reference on character set describing UTF-8 code page
	* This code page is default UNICODE representation in POSIX systems i.e. Linux, FreeBSD, Max OS X etc
	* \return reference on character set describing UTF-8 code page
	* \throw never throws
	*/
	static const Charset* utf8() BOOST_NOEXCEPT_OR_NOTHROW;

private:
	ctmap_t nameMap_;
	static boost::mutex _mutex;
	static boost::atomic<Charsets*> _instance;
};


} // namesapce io

#endif // __IO_CHARSETS_HPP_INCLUDED__
