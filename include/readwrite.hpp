#ifndef READER_HPP_INCLUDED
#define READER_HPP_INCLUDED

#include <stdexcept>
#include <charbuffers.hpp>
#include <convert.hpp>
#include <channels.hpp>

namespace io {

//template<class String>
//class SimpleReader {
//private:
//	typedef typename String::value_type _TChar;
//public:
//
//};

template<class String>
class Reader {
private:
	typedef typename String::value_type _TChar;
public:
	Reader(PReadChannel src, const byte_buffer& buff,PConverter conv) BOOST_NOEXCEPT_OR_NOTHROW:
		src_(src),
		buff_(buff),
		conv_(conv)
	{}
	String read() throw(io_exception,charset_exception) {
		buff_.clear();
		std::size_t bytesRead = src_->read(buff_);
		String result(bytesRead,0x1);
		byte_buffer conv = wrap_string(result);
		buff_.flip();
		conv_->convert(buff_,conv);
		return result;
	}
private:
	PReadChannel src_;
	byte_buffer buff_;
	PConverter conv_;
};

template<class String>
class Writer {
private:
	typedef typename String::value_type _TChar;
public:
	Writer(PWriteChannel out,PConverter conv) BOOST_NOEXCEPT_OR_NOTHROW:
		out_(out),
		conv_(conv)
	{}
	void write(const String& str) throw(io_exception,charset_exception) {
		std::size_t sourceBytesSize = str.length()*sizeof(_TChar);
		byte_buffer srcBytes = wrap_string(str);
		const std::size_t destCharSize = conv_->destCharset()->charSize();
		byte_buffer convBytes = new_byte_byffer(str.length()*destCharSize);
		conv_->convert(srcBytes, convBytes);
		convBytes.flip();
		out_->write(convBytes);
	}
private:
	PWriteChannel out_;
	PConverter conv_;
};

} // namespace io

#endif // READER_HPP_INCLUDED
