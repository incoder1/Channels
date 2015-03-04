#ifndef WIN32_CONV_HPP_INCLUDE
#define WIN32_CONV_HPP_INCLUDE

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/once.hpp>

#include <convert.hpp>
#include <smallobjectpool.hpp>

#include <Objbase.h>
#include <Mlang.h>

namespace io {

// Smart reference upon IMLangConvertCharset COM object
class PMLang {
private:
	IMLangConvertCharset *ptr_;
	inline void copyPtr(const PMLang& oth);
public:
	PMLang(::IMLangConvertCharset *converter) BOOST_NOEXCEPT;
	PMLang(const PMLang& oth);
	const PMLang& operator=(const PMLang& oth);
	~PMLang();
	inline ::IMLangConvertCharset* operator->() const {
		return ptr_;
	}
};

// Singleton for allocating COM and MLang DLL library (really have to be a singleton)
class MLangEngine:private boost::noncopyable {
private:
	HMODULE libMLang_;
	bool freeCOM_;
	static boost::once_flag _flag;
	static MLangEngine* _instance;
	// call is already protected by memory barrier
	static void createInstance() {
		_instance = new MLangEngine();
	}
	MLangEngine();
public:
	~MLangEngine() BOOST_NOEXCEPT_OR_NOTHROW;

	PMLang createConveter(const Charset *src, const Charset *dst) const throw(charset_exception);

	static const MLangEngine* instance() {
		// using call once to use barrier
		boost::call_once(&createInstance, _flag);
		return _instance;
	}
};

/**
 * ! \brief Microsoft MLang (Internet Explorer component) based converter
 */
class CHANNEL_PUBLIC Win32Converter : public virtual Converter, public virtual SmallObject {
public:
	Win32Converter(PMLang engine,const Charset* srcCt,const Charset* dstCt) BOOST_NOEXCEPT_OR_NOTHROW;
	~Win32Converter() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ssize_t convert(const byte_buffer& src, byte_buffer& dest) throw(charset_exception);
private:
	PMLang engine_;
	const Charset* srcCt_;
	const Charset* dstCt_;
};


} // namespace io

#endif // WIN32_CONV_HPP_INCLUDE
