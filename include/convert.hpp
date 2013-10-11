#ifndef CONVERT_HPP_INCLUDED
#define CONVERT_HPP_INCLUDED

#include <channels_config.h>
#include <bytebuffer.hpp>
#include <charbuffers.hpp>
#include <bytebuffer.hpp>

#include <boost/unordered_map.hpp>
#include <stdexcept>

namespace io {

/**
 * ! \brief Represent basic information about character set
 */
class CHANNEL_PUBLIC Charset {
private:
	const size_t id_;
	std::string name_;
	const size_t charSize_;
public:
	/**
	 * Constructs character set
	 * \param id
	 *			the unique code page identifier (Win32 API identifier)
	 * \param name
	 *			the unique name of character set (libiconv/posix name)
	 *	\param charSize
	 *		the size in bytes of single character in this character set
	 */
	explicit Charset(size_t id, const std::string& name, const size_t charSize) BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	 * Returns code page identifier
	 * \return code page identifier
	 */
	const size_t id() const BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	 * Returns unique name of character set
	 * \return unique name of character set
	 */
	const std::string name() const BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	 * Returns size in bytes of single character
	 * \return size in bytes of single character
	 */
	const size_t charSize() const BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	 * Compare this charset with a pointer to the another.
	 * \param oth
	 *			constant pointer to the charset
	 * \return whether charsets are equal. If {@code oth} is {@code nullptr} return false
	 */
	bool equal(const Charset* oth) const;
};

// Not available from DLL, do not use it
class CHANNEL_PUBLIC CharsetFactory {
private:
	typedef boost::unordered_map<std::string,const Charset*> hash_table_t;
	hash_table_t charSets_;
public:
	CharsetFactory() BOOST_NOEXCEPT_OR_NOTHROW;
	const Charset* forName(const std::string& name) const;
};

/**
 * ! \brief Exception of this type trowing when character set conversation fails.
 */
class CHANNEL_PUBLIC charset_exception:public std::exception {
private:
	const std::string msg_;
public:
	charset_exception(const std::string& message);
	virtual const char* what() const BOOST_NOEXCEPT;
};

/**
 * ! \brief  Implementor of {@code Converter} should provide conversation from one character set into another
 */
class CHANNEL_PUBLIC Converter {
public:
	/**
	 * Returns source character set
	 * \return constant pointer to the source character set
	 */
	virtual const Charset* sourceCharset() const = 0;

	/**
	 * Returns destination character set
	 * \return constant pointer to the destination character set
	 */
	virtual const Charset* destinationCharset() const = 0;

	/**
	 * Converting character sequence from source character set into destination charter set
	 * \param src
	 *			source characters in their bytes sequence representation
	 * \param dest
	 *			destination characters in their bytes sequence representation
	 * \throw charset_exception
	 *			when conversation fails, the reason can be obtained by what method
	 */
	virtual void convert(const byte_buffer& src, byte_buffer& dest) throw(charset_exception) = 0;

	/**
	 * Frees resources allocated by converter
	 */
	virtual ~Converter() = 0;
};

/**
 * Smart pointer to the Coverter type, used by API witch need conversation like
 * {@link Reader} or {@link Writer}.
 */
typedef boost::shared_ptr<Converter> PConverter;

} // namespace io

#endif // CONVERT_HPP_INCLUDED
