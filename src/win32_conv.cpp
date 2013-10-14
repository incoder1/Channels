#include "win32_conv.hpp"


namespace io {

PMLang::PMLang() BOOST_NOEXCEPT:
	ptr_(NULL)
{}

inline void PMLang::copyPtr(const PMLang& oth)
{
	if(NULL != oth.ptr_) {
		oth.ptr_->AddRef();
	}
	ptr_ = oth.ptr_;
}

PMLang::PMLang(const PMLang& oth) BOOST_NOEXCEPT {
	copyPtr(oth);
}

const PMLang& PMLang::operator=(const PMLang& oth) BOOST_NOEXCEPT {
	copyPtr(oth);
	return *this;
}

inline void PMLang::reset(IMLangConvertCharset *ptr) BOOST_NOEXCEPT {
	if(NULL != ptr_) {
		while(ptr_->Release());
	}
	ptr_ = ptr;
}

inline IMLangConvertCharset* PMLang::operator->() const BOOST_NOEXCEPT
{
	return ptr_;
}

inline IMLangConvertCharset* PMLang::operator&() const BOOST_NOEXCEPT
{
	return ptr_;
}

PMLang::~PMLang()
{
	if(NULL != ptr_) {
		ptr_->Release();
	}
}


PConverter win32_converter(const std::string& src, const std::string& dst) throw(charset_exception)
{
	static CharsetFactory chFactory;
	if(src == dst) {
		throw charset_exception("Source character set is equal destination, no conversation needed");
	}
	const Charset* srcCt = chFactory.forName(src);
	if(NULL == srcCt) {
		throw charset_exception(src+" is not provided by iconv converter");
	}
	const Charset* destCt = chFactory.forName(dst);
	if(NULL == srcCt) {
		throw charset_exception(dst+" is not provided by iconv converter");
	}
	return PConverter(new Win32Converter(srcCt,destCt));
}

Win32Converter::Win32Converter(const Charset* srcCt,const Charset* dstCt) BOOST_NOEXCEPT_OR_NOTHROW:
		 libMLang_(NULL),
		 freeCOM_(false),
         srcCt_(srcCt),
         dstCt_(dstCt)
{
	libMLang_ = ::LoadLibraryW(L"mlang.dll");
	freeCOM_ = (S_OK == ::CoInitializeEx(NULL,  COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE) );
	::IMLangConvertCharset *cvt = NULL;
	HRESULT res = ::CoCreateInstance(CLSID_CMLangConvertCharset, NULL, CLSCTX_INPROC_SERVER, IID_IMLangConvertCharset,(void**)(&cvt) );
	if(SUCCEEDED(res)) {
		mLang_.reset(cvt);;
	}
}

void Win32Converter::convert(const byte_buffer& src, byte_buffer& dest) throw(charset_exception)
{

	if(! SUCCEEDED( mLang_->Initialize(srcCt_->id(),dstCt_->id(),0) ) ) {
		throw charset_exception("Can not initialize converter engine MS MLang");
	}

	BYTE *itptr = const_cast<BYTE*>(&src.position());
	BYTE *dstptr = const_cast<BYTE*>(&dest.position());
	UINT srclen = src.length();
	UINT avail = dest.capacity();
	if(!SUCCEEDED(mLang_->DoConversion(itptr, &srclen, dstptr, &avail))) {
		throw charset_exception("MLang character conversation failed");
	}
	// calc size of char buffer, and move it
	size_t offset = dest.capacity() - avail;
	dest.move(offset);
}

const Charset* Win32Converter::sourceCharset() const
{
	return srcCt_;
}

const Charset* Win32Converter::destinationCharset() const
{
	return dstCt_;
}

Win32Converter::~Win32Converter()
{
	if(freeCOM_) {
		::CoUninitialize();
	}
	::FreeLibrary(libMLang_);
}

} // namespace io
