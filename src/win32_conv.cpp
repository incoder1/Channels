#include "win32_conv.hpp"

namespace io {

boost::once_flag MLangEngine::_flag;
MLangEngine* MLangEngine::_instance = NULL;

//MLangEngine
MLangEngine::MLangEngine() BOOST_NOEXCEPT_OR_NOTHROW:
	libMLang_(NULL),
	freeCOM_(false)
{
	libMLang_ = ::LoadLibraryW(L"mlang.dll");
	freeCOM_ = (S_OK == ::CoInitializeEx(NULL,  COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE) );
}

MLangEngine::~MLangEngine()
{
	if(freeCOM_) {
		::CoUninitialize();
	}
	if(NULL != libMLang_) {
		::FreeLibrary(libMLang_);
	}
}

inline void validate_charset(const Charset* ch, const std::string& name) throw(charset_exception)
{
	validate(NULL == ch, name + " is not provided by MLang converter");
}

inline void validate_engine(HRESULT status) throw(charset_exception)
{
	validate(FAILED(status),"Can not construct MLang Engine instance");
}

PMLang MLangEngine::createConveter(const Charset *src, const Charset *dst) const throw(charset_exception)
{
	::IMLangConvertCharset *cvt = NULL;
	HRESULT status = ::CoCreateInstance(CLSID_CMLangConvertCharset, NULL, CLSCTX_INPROC_SERVER, IID_IMLangConvertCharset,(void**)(&cvt));
	validate_engine(status);
	status = cvt->Initialize(src->id(), dst->id(), 0);
	validate_engine(status);
	return PMLang(cvt);
}

PMLang::PMLang(IMLangConvertCharset* cvt) BOOST_NOEXCEPT:
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


SConverter CHANNEL_PUBLIC win32_converter(const char* src, const char* dst) throw(charset_exception)
{
	static CharsetFactory chFactory;
	const Charset* srcCt = chFactory.forName(src);
	validate_charset(srcCt, src);
	const Charset* destCt = chFactory.forName(dst);
	validate_charset(destCt, dst);
	if(srcCt->equal(destCt) ) {
		boost::throw_exception(charset_exception("Source character set is equal destination, no conversation needed"));
	}
	PMLang conv = MLangEngine::instance()->createConveter(srcCt, destCt);
	return SConverter(new Win32Converter(conv, srcCt,destCt));
}

Win32Converter::Win32Converter(PMLang engine,const Charset* srcCt,const Charset* dstCt) BOOST_NOEXCEPT_OR_NOTHROW:
		Converter(srcCt,dstCt),
		engine_(engine)
{}

void Win32Converter::convert(const byte_buffer& src, byte_buffer& dest) throw(charset_exception)
{
	BYTE *srcptr = const_cast<BYTE*>(&src.position());
	BYTE *dstptr = const_cast<BYTE*>(&dest.position());
	UINT srclen = src.length();
	UINT avail = dest.capacity();
	HRESULT status = engine_->DoConversion(srcptr, &srclen, dstptr, &avail);
	validate(status != S_OK , "MLang character conversation failed");
	// calc size of char buffer, and move it
	std::size_t offset = dest.capacity() - avail;
	dest.move(0 != offset ? offset: dest.capacity() - 1);
}

Win32Converter::~Win32Converter() BOOST_NOEXCEPT_OR_NOTHROW
{
}

} // namespace io
