#ifndef WIN32_CONV_HPP_INCLUDE
#define WIN32_CONV_HPP_INCLUDE

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/once.hpp>

#include <smallobject.hpp>
#include <charsets.hpp>
#include <bytebuffer.hpp>

#include <Objbase.h>
#include <Mlang.h>

namespace io {

class MLangFactory;
// Smart reference upon IMLangConvertCharset COM object
class PMLang {
private:
	inline void copyPtr(const PMLang& oth);
	PMLang(::IMLangConvertCharset *converter) BOOST_NOEXCEPT_OR_NOTHROW;
	friend class MLangFactory;
public:
	PMLang(const PMLang& oth);
	const PMLang& operator=(const PMLang& oth);
	~PMLang();
	inline ::IMLangConvertCharset* operator->() const {
		return ptr_;
	}
private:
	IMLangConvertCharset *ptr_;
};

/**
 * ! \brief Microsoft MLang (Internet Explorer component) based converter
 */
class CHANNEL_PUBLIC MLangConverter : public object {
private:
	inline std::size_t calcBuffSize(const byte_buffer& src);
public:
	MLangConverter(const Charset* from,const Charset* to);
	byte_buffer convert(const byte_buffer& src) throw(std::bad_alloc, std::runtime_error);
	std::size_t convert(const byte_buffer& src, byte_buffer& dest) throw(std::runtime_error);
private:
	const Charset* from_;
	const Charset* to_;
	PMLang engine_;
};

typedef boost::shared_ptr<MLangConverter> SConverter;

SConverter CHANNEL_PUBLIC make_converter(const Charset* from, const Charset* to) throw (std::bad_alloc,std::runtime_error);

inline SConverter make_converter(const char *from, const char* to) {
	return make_converter(Charsets::forName(from),Charsets::forName(to));
}

} // namespace io

#endif // WIN32_CONV_HPP_INCLUDE
