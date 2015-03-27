#include "prchdrs.h"
#include "msmlang_conv.hpp"

#ifndef __IID_DEFINED__
#define __IID_DEFINED__
typedef struct _IID {
	unsigned long x;
	unsigned short s1;
	unsigned short s2;
	unsigned char  c[8];
} IID;
#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#ifndef _MIDL_USE_GUIDDEF_
#	define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}
#endif // !_MIDL_USE_GUIDDEF_

extern "C" {
	MIDL_DEFINE_GUID(IID, IID_IMLangConvertCharset,0xd66d6f98,0xcdaa,0x11d0,0xb8,0x22,0x00,0xc0,0x4f,0xc9,0xb3,0x1f);
	MIDL_DEFINE_GUID(CLSID, CLSID_CMLangConvertCharset,0xd66d6f99,0xcdaa,0x11d0,0xb8,0x22,0x00,0xc0,0x4f,0xc9,0xb3,0x1f);
}

namespace io {

inline void validate_charset(const Charset* ch, const std::string& name) throw(std::runtime_error)
{
	validate<std::runtime_error>(NULL != ch, name + " is not provided by MLang converter");
}

inline void validate_engine(HRESULT status) throw(std::runtime_error)
{
	validate<std::runtime_error>(SUCCEEDED(status),"Can not construct MLang Engine instance");
}

// Singleton for allocating COM and MLang DLL library (really have to be a singleton)
class MLangFactory:private boost::noncopyable {
private:
	static void initialize() {
		_instance = new MLangFactory();
		std::atexit(&MLangFactory::destoroy);
	}
	static void destoroy() {
		delete _instance;
	}
	MLangFactory():
		libMLang_(NULL),
		freeCOM_(false)
	{
		libMLang_ = ::LoadLibraryW(L"mlang.dll");
		freeCOM_ = (S_OK == ::CoInitializeEx(NULL,  COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE) );
	}
public:
	~MLangFactory() {
		if(freeCOM_) {
			::CoUninitialize();
		}
		if(NULL != libMLang_) {
			::FreeLibrary(libMLang_);
		}
	}


	PMLang createConveter(const Charset *src, const Charset *dst) const throw(std::runtime_error)
	{
		::IMLangConvertCharset *cvt = NULL;
		HRESULT status = ::CoCreateInstance(CLSID_CMLangConvertCharset, NULL, CLSCTX_INPROC_SERVER, IID_IMLangConvertCharset,reinterpret_cast<void**>(&cvt));
		validate_engine(status);
		status = cvt->Initialize(src->id(), dst->id(), 0);
		validate_engine(status);
		return PMLang(cvt);
	}

	static const MLangFactory* instance() {
		// using call once to use barrier
		boost::call_once(&MLangFactory::initialize, _onceFlag);
		return _instance;
	}
private:
	HMODULE libMLang_;
	bool freeCOM_;
	static boost::once_flag _onceFlag;
	static MLangFactory* _instance;
};

boost::once_flag MLangFactory::_onceFlag = BOOST_ONCE_INIT;
MLangFactory* MLangFactory::_instance = NULL;



PMLang::PMLang(IMLangConvertCharset* cvt) BOOST_NOEXCEPT_OR_NOTHROW:
	ptr_(cvt)
{}

inline void PMLang::copyPtr(const PMLang& oth)
{
	if(NULL != oth.ptr_) {
		oth.ptr_->AddRef();
	}
	ptr_ = oth.ptr_;
}

PMLang::PMLang(const PMLang& oth)
{
	copyPtr(oth);
}

const PMLang& PMLang::operator=(const PMLang& oth)
{
	copyPtr(oth);
	return *this;
}

PMLang::~PMLang()
{
	if(NULL != ptr_) {
		ptr_->Release();
	}
}

// MLangConverter
MLangConverter::MLangConverter(const Charset* from,const Charset* to):
		object(),
		from_(from),
		to_(to),
		engine_(MLangFactory::instance()->createConveter(from,to))
{
}

std::size_t MLangConverter::convert(const byte_buffer& src, byte_buffer& dest) throw(std::runtime_error)
{
	BYTE *srcptr = const_cast<BYTE*>(src.position().ptr());
	BYTE *dstptr = const_cast<BYTE*>(dest.position().ptr());
	UINT srclen = src.length();
	UINT avail = dest.capacity();
	HRESULT status = engine_->DoConversion(srcptr, &srclen, dstptr, &avail);
	validate<std::runtime_error>(status == S_OK , "MLang character conversation failed");
	// calc size of char buffer, and move it
	if(avail > 0) {
		dest.clear();
		ptrdiff_t offset = (uint8_t*)dstptr - dest.position().ptr();
		dest.move((std::size_t)offset);
	} else {
		dest.move(dest.end());
	}
	dest.flip();
	return dest.length();
}

inline std::size_t MLangConverter::calcBuffSize(const byte_buffer& src) {
	std::size_t result = 0;
	if(to_->charSize() > from_->charSize() || from_ == Charsets::utf8() ){
		result = src.length() * to_->charSize();
	} else {
		result= src.length();
	}
	return result;
}

byte_buffer MLangConverter::convert(const byte_buffer& src) throw(std::bad_alloc, std::runtime_error)
{
	std::size_t buffSize = calcBuffSize(src);
	byte_buffer dest = byte_buffer::heap_buffer(buffSize);
	convert(src,dest);
	return dest;
}

SConverter CHANNEL_PUBLIC make_converter(const Charset* from, const Charset* to) throw(std::bad_alloc,std::runtime_error)
{
	validate<std::runtime_error>(!from->equal(to), "Can not convert to same character sets");
	return boost::make_shared<MLangConverter>(from, to);
}

} // namespace io
