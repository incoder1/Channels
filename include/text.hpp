#ifndef READER_HPP_INCLUDED
#define READER_HPP_INCLUDED

#include <stdexcept>
#include <convert.hpp>
#include <channels.hpp>

#include <boost/format.hpp>

namespace io {

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
		byte_buffer buff = byte_buffer::wrap_array(str.data(), str.length()); // 0 ending symbol should be avoided
		write(buff);
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

	void write(const byte_buffer& buff) {
		byte_buffer b(buff);
		while(out_->write(b) > 0);
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
typedef basic_reader<int16_t> u16reader;
typedef basic_reader<int32_t> u32reader;
typedef conv_reader<char> cnv_reader;
typedef conv_reader<wchar_t> cnv_wreader;
typedef conv_reader<int16_t> cnv_u16reader;
typedef conv_reader<int32_t> cnv_u32wreader;

typedef basic_writer<char> writer;
typedef basic_writer<wchar_t> wwriter;
typedef basic_writer<int16_t> u16writer;
typedef basic_writer<int32_t> u32writer;
typedef conv_writer<char> cnv_writer;
typedef conv_writer<wchar_t> cnv_wwriter;
typedef conv_writer<int16_t> cnv_u16writer;
typedef conv_writer<int32_t> cnv_u32writer;


} // namespace io

#endif // READER_HPP_INCLUDED
