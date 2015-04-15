#ifndef __IO_TEXT_HPP_INCLUDED__
#define __IO_TEXT_HPP_INCLUDED__

#include <stdexcept>
#include <convert.hpp>
#include <channels.hpp>

#include <boost/format.hpp>


namespace io {

#ifdef PLATFORM_WINDOWS
const char* AENDL = "\n\r";
const wchar_t* WENDL = L"\n\r";
#	ifndef BOOST_NO_CHAR16_T
const char16_t* uENDL = u"\n\r";
#	endif
#	ifndef BOOST_NO_CHAR32_T
const char32_t* UENDL = U"\n\r";
#	endif
#else
const char* AENDL = "\n";
const wchar_t* WENDL = L"\n";
#	ifndef BOOST_NO_CHAR16_T
const char16_t* uENDL = u"\n";
#	endif
#	ifndef BOOST_NO_CHAR32_T
const char32_t UENDL = U"\n";
#	endif
#endif

template<typename _char_t, class _traits_t = std::char_traits<_char_t> >
class basic_reader {
public:
	typedef std::basic_string<_char_t, _traits_t> string_t;
	basic_reader(SReadChannel src) BOOST_NOEXCEPT_OR_NOTHROW:
		src_(src)
	{}
	std::size_t read(byte_buffer& dest) {
		std::size_t result = src_->read(dest);
		dest.flip();
		return result;
	}
	string_t read(std::size_t max) {
		string_t result(0,max);
		byte_buffer buff = byte_buffer::wrap_array(result.data(),max);
		src_->read(buff);
		return result;
	}
private:
	SReadChannel src_;
};

template<typename _char_t>
class conv_reader :public basic_reader<_char_t> {
public:
	typedef typename basic_reader<_char_t>::string_t string_t;
	conv_reader(SReadChannel src,SConverter conv) BOOST_NOEXCEPT_OR_NOTHROW:
		basic_reader<_char_t>(src),
		conv_(conv)
	{}
	std::size_t read(byte_buffer& dest) {
		byte_buffer src = byte_buffer::heap_buffer(dest.capacity());
		std::size_t result = read_raw(src);
		return result > 0 ? result : conv_->convert(src, dest);
	}
	std::size_t read_raw(byte_buffer& dest) {
		return basic_reader<_char_t>::read(dest);
	}
	string_t read(std::size_t max) {
		string_t result(0,max);
		byte_buffer buff = byte_buffer::wrap_array(result.data(),max);
		read(buff);
		return result;
	}
private:
	SConverter conv_;
};

/**
 * ! \brief Writer temple, provides functionality for writing strings into write channel.
 * Strings can be converted from one character set into another
 */
template<typename _char_t, class _traits_t = std::char_traits<_char_t> >
class basic_writer {
public:
	typedef std::basic_string<_char_t, _traits_t> string;

	typedef boost::basic_format<_char_t, _traits_t, std::allocator<_char_t> > format;

	basic_writer(SWriteChannel out) BOOST_NOEXCEPT_OR_NOTHROW:
		out_(out)
	{}

	/**
	 * Writes STL string into write channel
	 */
	void write(const string& str) {
		write(str.data());
	}

	void write(_char_t* const v, std::size_t size) {
		write(byte_buffer::wrap_array(v, size));
	}

	void write(const _char_t* cStr) {
		write(const_cast<_char_t*>(cStr),_traits_t::length(cStr));
	}

	void write(const format& format) {
		write(format.str());
	}

	void writeln(const char* cStr) {
		write(cStr);
		write(AENDL);
	}

	void writeln(const wchar_t* cStr) {
		write(cStr);
		write(WENDL);
	}

#ifndef BOOST_NO_CHAR16_T
	void writeln(const char16_t* cStr) {
		write(cStr);
		write(uENDL);
	}
#endif // BOOST_NO_CHAR16_T

#ifndef BOOST_NO_CHAR32_T
	void writeln(const char32_t* cStr) {
		write(cStr);
		write(UENDL);
	}
#endif // BOOST_NO_CHAR32_T

	void write(const byte_buffer& buff) {
		byte_buffer b(buff);
		while(b.last() != b.position()+1) {
			b.move(out_->write(b));
		};
	}

private:
	SWriteChannel out_;
};

template<typename _char_t, class _traits_t = std::char_traits<_char_t> >
class conv_writer:public basic_writer<_char_t, _traits_t> {
public:

	typedef typename basic_writer<_char_t,_traits_t>::string string;
	typedef typename basic_writer<_char_t,_traits_t>::format format;

	/**
	 * Constructs new writer to write converted string.
	 * Note, use an empty converter in case when conversation is not needed
	 * \param out smart pointer to the write channel
	 * \param conv smart pointer ti the character converter
	 */
	conv_writer(SWriteChannel out,SConverter conv) BOOST_NOEXCEPT_OR_NOTHROW:
		basic_writer<_char_t, _traits_t>(out),
		conv_(conv)
	{}

	/**
	 * Writes STL string into write channel, with character conversation
	 */
	void write(const string& str) {
		write(byte_buffer::wrap_array(str.data(), str.length()));
	}

	void write_raw(_char_t* const v, std::size_t size) {
		basic_writer<_char_t>::write(v,size);
	}

	void write(_char_t* const v, std::size_t size) {
		write(byte_buffer::wrap_array(v, size));
	}

	void write(const _char_t* cStr) {
		write(const_cast<_char_t*>(cStr),_traits_t::length(cStr));
	}

	void write(const format& format) {
		write(format.str());
	}

	/**
	 * Writes content of the byte buffer into write channel
	 */
	void write(const byte_buffer& buff) {
		byte_buffer raw(buff);
		byte_buffer convBytes = conv_->convert(raw);
		basic_writer<_char_t>::write(convBytes);
	}
private:
	SConverter conv_;
};

typedef basic_reader<char> reader;
typedef basic_reader<wchar_t> wreder;
typedef conv_reader<char> cnv_reader;
typedef conv_reader<wchar_t> cnv_wreader;

typedef basic_writer<char> writer;
typedef basic_writer<wchar_t> wwriter;
typedef conv_writer<char> cnv_writer;
typedef conv_writer<wchar_t> cnv_wwriter;


#ifndef BOOST_NO_CHAR16_T
typedef basic_reader<char16_t> u16reader;
typedef basic_writer<char16_t> u16writer;
typedef conv_reader<char16_t> cnv_u16reader;
typedef conv_writer<char16_t> cnv_u16writer;
#endif
#ifndef BOOST_NO_CHAR32_T
typedef basic_reader<char32_t> u32reader;
typedef basic_writer<char32_t> u32writer;
typedef conv_reader<char32_t> cnv_u32reader;
typedef conv_writer<char32_t> cnv_u32writer;
#endif



} // namespace io

#endif // __IO_TEXT_HPP_INCLUDED__
