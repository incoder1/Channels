#ifndef __IO_TEXT_HPP_INCLUDED__
#define __IO_TEXT_HPP_INCLUDED__

#include <stdexcept>
#include <convert.hpp>
#include <channels.hpp>

#include <boost/format.hpp>


namespace io {


#ifdef PLATFORM_WINDOWS

template<typename _char_t>
struct endl
{};

template<>
struct endl<char> {
	static const char* id() {
		return "\n\r";
	}
};
template<>
struct endl<wchar_t> {
	static const wchar_t* id() {
		return L"\n\r";
	}
};
#	ifndef BOOST_NO_CHAR16_T
template<>
struct endl<char16_t> {
	static const char16_t* id() {
		return u"\n\r";
	}
};
#	endif // HAS_CHAR16_T
#	ifndef BOOST_NO_CHAR32_T
template<>
struct endl<char32_t> {
	static const char32_t* id() {
		return U"\n\r";
	}
};
#	endif // HAS_CHAR32_T
#else
template<>
struct endl<char> {
	static const char* id() {
		return "\n";
	}
};
template<>
struct endl<wchar_t> {
	static const wchar_t* id() {
		return L"\n";
	}
};
#	ifndef BOOST_NO_CHAR16_T
template<>
struct endl<char16_t> {
	static const char16_t* id() {
		return u"\n";
	}
};
#	endif // HAS_CHAR16_T
#	ifndef BOOST_NO_CHAR32_T
template<>
struct endl<char32_t> {
	static const char32_t* id() {
		return U"\n";
	}
};
#	endif // HAS_CHAR32_T
#endif

template<typename _char_t, class _traits_t = std::char_traits<_char_t> >
class basic_reader {
public:
	typedef std::basic_string<_char_t, _traits_t> string_t;
	basic_reader(SReadChannel src) BOOST_NOEXCEPT_OR_NOTHROW:
		src_(src)
	{}
	inline std::size_t load(byte_buffer& dest) {
		std::size_t result = src_->read(dest);
		dest.flip();
		return result;
	}
	string_t read(std::size_t max) {
		string_t result(0,max);
		load(byte_buffer::wrap_array(result.data(),max));
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
		std::size_t result =  basic_reader<_char_t>::load(src);
		return result ?  conv_->convert(src, dest) : result;
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

	inline void write(_char_t* const v, std::size_t size) {
		flush(byte_buffer::wrap_array(v, size));
	}

	inline void write(const _char_t* cStr) {
		write(const_cast<_char_t*>(cStr),_traits_t::length(cStr));
	}

	inline void write(const format& format) {
		write(format.str());
	}

	inline void writeln(const _char_t* cStr) {
		write(cStr);
		write(endl<_char_t>::id());
	}

	/**
	 * Writes STL string into write channel
	 */
	inline void write(const string& str) {
		flush(byte_buffer::wrap_array(str.data(),str.length()));
	}

	inline void writeln(const string& str) {
		write(str);
		write(endl<_char_t>::id());
	}

	inline void writeln(const format& format) {
		writeln(format.str());
	}

	/**
	 * Flushes a raw bytes into underlying channel
	 * \param buff a memory buffer, which is holding the data
	 */
	void flush(const byte_buffer& buff) {
		if(buff.empty()) return;
		byte_buffer b(buff);
		while(b.last() != b.position()+1) {
			b.move(out_->write(b));
		};
	}

	/**
	 * Flushes a raw bytes into underlying channel
	 * \param arr address to memory lined memory with data to flush
	 * \param size size of memory block in bytes
	 */
	template<typename element_type>
	void flush(element_type* const arr,std::size_t size) {
		flush(byte_buffer::wrap_array(reinterpret_cast<uint8_t*>(arr),size*sizeof(element_type)));
	}

private:
	SWriteChannel out_;
};


template<typename _char_t, class _traits_t = std::char_traits<_char_t> >
class conv_writer:basic_writer<_char_t,_traits_t> {
public:
	typedef std::basic_string<_char_t, _traits_t> string;

	typedef boost::basic_format<_char_t, _traits_t, std::allocator<_char_t> > format;

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

	inline void write(_char_t* const v, std::size_t size) {
		write(byte_buffer::wrap_array(v,size));
	}

	inline void write(const _char_t* cStr) {
		write(const_cast<_char_t*>(cStr),_traits_t::length(cStr));
	}

	inline void writeln(const _char_t* cStr) {
		write(cStr);
		write(endl<_char_t>::id());
	}

	/**
	 * Writes STL string into write channel, with character conversation
	 */
	inline void write(const string& str) {
		write(byte_buffer::wrap_array(str.data(), str.length()));
	}

	inline void write(const format& format) {
		write(format.str());
	}

	/**
	 * Writes content of the byte buffer into write channel
	 */
	inline void write(const byte_buffer& buff) {
		byte_buffer raw(buff);
		byte_buffer convBytes = conv_->convert(raw);
		basic_writer<_char_t, _traits_t>::flush(convBytes);
	}

	inline void writeln(const string& str) {
		write(str);
		write(endl<_char_t>::id());
	}

	inline void writeln(const format& format) {
		writeln(format.str());
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
