#ifndef CONVERT_HPP_INCLUDED
#define CONVERT_HPP_INCLUDED

#include <channels_config.h>
#include <bytebuffer.hpp>
#include <charbuffers.hpp>
#include <bytebuffer.hpp>

#include <boost/unordered_map.hpp>
#include <boost/exception/all.hpp>
#include <stdexcept>

namespace io {

/**
 * ! \brief Represent basic information about character set
 */
class CHANNEL_PUBLIC Charset {
private:
	const std::size_t id_;
	const char* name_;
	const std::size_t charSize_;
	bool unicode_;
private:
	Charset(const Charset& c):id_(c.id_),name_(c.name_),charSize_(c.charSize_),unicode_(false)
	{}
	const Charset& operator=(const Charset&) {
		return *this;
	}
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
	 *			the unique code page identifier (Win32 API identifier)
	 * \param name
	 *			the unique name of character set (libiconv/posix name)
	 *	\param charSize
	 *		the size in bytes of single character in this character set
	 */
	Charset(std::size_t id, const char* name, const std::size_t charSize) BOOST_NOEXCEPT_OR_NOTHROW;

	/**
	 * Returns code page identifier (the value is the same as Win32 API code page)
	 * \return code page identifier
	 */
	std::size_t id() const;
	/**
	 * Returns unique name of character set
	 * \return unique name of character set
	 */
	const char* name() const;
	/**
	 * Returns size in bytes of single character
	 * \return size in bytes of single character
	 */
	std::size_t  charSize() const;

	/**
	 * Return whether current char set is Unicode code-page representation
	 * \return whether current char set is Unicode code-page representation
	 */
	bool isUnicode() const;

	/**
	 * Compare this charset with a pointer to the another
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
	const Charset* forName(const char* name) const;
};

/**
 * ! \brief Exception of this type trowing when character set conversation fails.
 */
class charset_exception:public std::exception {
private:
	const std::string msg_;
public:
	charset_exception(const std::string& message):
		std::exception(),
		msg_(message)
	{}
	~charset_exception() BOOST_NOEXCEPT_OR_NOTHROW
	{}
	virtual const char* what() const BOOST_NOEXCEPT_OR_NOTHROW {
		return msg_.c_str();
	}
};

// Helper function used to throw char set error when expression
inline void validate(bool expr, const std::string& msg) throw(charset_exception)
{
	if(expr) {
		boost::throw_exception(charset_exception(msg));
	}
}

/**
 * ! \brief  Implementor of {@code Converter} should provide conversation from one character set into another
 */
class CHANNEL_PUBLIC Converter {
private:
	const Charset* srcCt_;
	const Charset* destCt_;
protected:
	Converter(const Charset* srcCt,const Charset* destCt) BOOST_NOEXCEPT_OR_NOTHROW:
		srcCt_(srcCt),
		destCt_(destCt)
	{}
public:

	/**
	 * Returns source character set
	 * \return constant pointer to the source character set
	 */
	const Charset* srcCharset() {
		return srcCt_;
	}

	/**
	 * Returns destination character set
	 * \return constant pointer to the destination character set
	 */
	const Charset* destCharset() {
		return destCt_;
	}

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


// Detecting engine
#ifdef CONV_ENGINE_ICONV
	PConverter CHANNEL_PUBLIC iconv_conv(const char* src, const char* dst) throw(charset_exception);
#elif defined(CONV_ENGINE_IBM_ICU)
	PConverter CHANNEL_PUBLIC icu_conv(const char* src, const char* dst) throw(charset_exception);
#else
	PConverter CHANNEL_PUBLIC win32_converter(const char* src, const char* dst) throw(charset_exception);
#endif // conv engine selection

/**
 * Creates new char set converter with current character set (code page) converting engine
 * \param src
 *			name of source character set
 * \param dst
 *			name of destination character set
 *	\return smart pointer to the converter
 *	\throw  charset_exception
 *				if conversation is not supported by engine, or engine can not be created
 */
inline PConverter new_converter(const char* src, const char* dst) throw(charset_exception) {
#ifdef CONV_ENGINE_ICONV
	return iconv_conv(src,dst);
#elif defined(CONV_ENGINE_IBM_ICU)
	return icu_conv(src,dst);
#else
	return win32_converter(src,dst);
#endif // conv engine selection
}

} // namespace io

#endif // CONVERT_HPP_INCLUDED
