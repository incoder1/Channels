#ifndef __INCONV_CONV_HPP_INCLUDED
#define __INCONV_CONV_HPP_INCLUDED

#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <cerrno>
#include <iconv.h>

#include "bytebuffer.hpp"
#include "charsets.hpp"
#include "smallobject.hpp"

namespace io {


/**
 * ! \brief Converts string representing in byte sequence from one code page (charset)
 *  to the another.
 */
class CHANNEL_PUBLIC IconvConverter:public object {
public:
	IconvConverter(const Charset* from, const Charset* to);
	~IconvConverter() BOOST_NOEXCEPT_OR_NOTHROW;

	/**
	 * Converts character sequence from source character set into destination charter set
	 * \param src source characters in their bytes sequence representation
	 *  \return heap byte buffer with converted characters
	 */
	byte_buffer convert(const byte_buffer& src) throw(std::bad_alloc,std::runtime_error);

	/**
	 * Converts character sequence from source character set into destination charter set
	 * \param src source characters in their bytes sequence representation
	 * \param dest destination characters in their character type representation
	 * \return converted string length in bytes
	 */
	std::size_t convert(const byte_buffer& src, byte_buffer& dest) throw(std::runtime_error);

private:
	inline std::size_t calcBuffSize(const byte_buffer& src);
private:
	boost::shared_ptr<void> conv_;
	const Charset* from_;
	const Charset* to_;
};

typedef boost::shared_ptr<IconvConverter> SConverter;

SConverter CHANNEL_PUBLIC make_converter(const Charset* from, const Charset* to);

inline SConverter make_converter(const char *from, const char* to) {
	return make_converter(Charsets::forName(from),Charsets::forName(to));
}

} // namespace io
#endif // __INCONV_CONV_HPP_INCLUDED
