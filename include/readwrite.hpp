#ifndef READER_HPP_INCLUDED
#define READER_HPP_INCLUDED

#include <stdexcept>
#include <charbuffers.hpp>
#include <convert.hpp>
#include <channels.hpp>

namespace io {

template<class String>
class Reader {
private:
	typedef typename String::value_type _TChar;
public:
Reader(PReadChannel src, const byte_buffer& buff,PConverter conv) BOOST_NOEXCEPT:
	src_(src),
	     buff_(buff),
	     conv_(conv)
	{}
	String read() throw(io_exception,charset_exception) {
		buff_.clear();
		size_t bytesRead = src_->read(buff_);
		byte_buffer convBuff = new_byte_byffer(bytesRead*sizeof(_TChar));
		buff_.flip();
		conv_->convert(buff_,convBuff);
		convBuff.flip();
		String res;
		const _TChar* start = (const _TChar*)(&convBuff.position());
		const _TChar* end = (const _TChar*)(&convBuff.last());
		res.append(start, end );
		return res;
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
Writer(PWriteChannel out,PConverter conv) BOOST_NOEXCEPT:
	out_(out),
	     conv_(conv)
	{}
	void write(const String& str) throw(io_exception,charset_exception) {
		size_t sourceBytesSize = str.length()*sizeof(_TChar);
		byte_buffer srcBytes = new_byte_byffer(sourceBytesSize);
		srcBytes.put((uint8_t*)(str.data()), sourceBytesSize);
		srcBytes.flip();
		const size_t destCharSize = conv_->destinationCharset()->charSize();
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
