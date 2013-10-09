#ifndef CONVERT_HPP_INCLUDED
#define CONVERT_HPP_INCLUDED

#include <channels_config.h>
#include <bytebuffer.hpp>
#include <charbuffers.hpp>
#include <bytebuffer.hpp>

#include <stdexcept>

namespace io {

class CHANNEL_PUBLIC Charset {
private:
	const size_t id_;
	std::string name_;
	const size_t charSize_;
public:
	explicit Charset(size_t id, const std::string& name, const size_t charSize) BOOST_NOEXCEPT_OR_NOTHROW;
	const size_t id() const BOOST_NOEXCEPT_OR_NOTHROW;
	const std::string name() const BOOST_NOEXCEPT_OR_NOTHROW;
	const size_t charSize() const BOOST_NOEXCEPT_OR_NOTHROW;
	bool equal(const Charset* oth) const;
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

class CHANNEL_PUBLIC Converter {
private:

public:

	virtual const Charset* sourceCharset() const = 0;

	virtual const Charset* destinationCharset() const = 0;

	/**
	 * Converting character sequence from source character set into destination charter set
	 * \param src source characters in their bytes sequence representation
	 * \param dest destination characters in their character type representation
	 */
	virtual void convert(const byte_buffer& src, byte_buffer& dest) throw(charset_exception) = 0;

	/**
	 * Frees resources allocated by converter
	 */
	virtual ~Converter() = 0;
};

typedef boost::shared_ptr<Converter> PConverter;

} // namespace io

#endif // CONVERT_HPP_INCLUDED
