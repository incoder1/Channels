#ifndef READER_HPP_INCLUDED
#define READER_HPP_INCLUDED

#include <stdexcept>
#include <convert.hpp>
#include <channels.hpp>

namespace io {


/**
 * ! \brief Writer temple, provides functionality for writing strings into write channel.
 * Strings can be converted from one character set into another
 */
template<typename _char_t>
class basic_writer {
public:
	typedef std::basic_string<_char_t> string_t;

	basic_writer(SWriteChannel out) BOOST_NOEXCEPT_OR_NOTHROW:
		out_(out)
	{}

	/**
	 * Writes STL string into write channel
	 */
	void write(const string_t& str) {
		byte_buffer buff = byte_buffer::wrap_array(str.data(), str.length()); // 0 ending symbol should be avoided
		write(buff);
	}

	void write(const byte_buffer& buff) {
		byte_buffer b(buff);
		while(out_->write(b) > 0);
	}

private:
	SWriteChannel out_;
};

template<typename _char_t>
class conv_writer:public basic_writer<_char_t> {
public:
	/**
	 * Constructs new writer to write converted string.
	 * Note, use an empty converter in case when conversation is not needed
	 * \param out smart pointer to the write channel
	 * \param conv smart pointer ti the character converter
	 */
	conv_writer(SWriteChannel out,SConverter conv) BOOST_NOEXCEPT_OR_NOTHROW:
		basic_writer<_char_t>(out),
		conv_(conv)
	{}

public:
	typedef typename basic_writer<_char_t>::string_t string_t;

	/**
	 * Writes STL string into write channel, with character conversation
	 */
	void write(const string_t& str) {
		write(byte_buffer::wrap_array(str.data(), str.length()));
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


typedef basic_writer<char> writer;
typedef basic_writer<wchar_t> wwriter;
typedef conv_writer<char> cnv_writer;
typedef conv_writer<wchar_t> cnv_wwriter;

} // namespace io

#endif // READER_HPP_INCLUDED
