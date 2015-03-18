#ifndef __INCONV_CONV_HPP_INCLUDED
#define __INCONV_CONV_HPP_INCLUDED

#include <stdexcept>
#include <boost/shared_ptr.hpp>

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
	 * Converting character sequence from source character set into destination charter set
	 * \param src source characters in their bytes sequence representation
	 * \param dest destination characters in their character type representation
	 */
	byte_buffer convert(const byte_buffer& src) throw(std::bad_alloc,std::runtime_error);
private:
	boost::shared_ptr<void> conv_;
	const Charset* from_;
	const Charset* to_;
};

typedef IconvConverter Converter;
typedef boost::shared_ptr<Converter> SConverter;

} // namespace io
#endif // __INCONV_CONV_HPP_INCLUDED
