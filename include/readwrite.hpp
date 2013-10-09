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
	inline String read() throw(io_exception) {
		buff_.clear();
		size_t bytesRead = src_->read(buff_);
		byte_buffer convBuff = new_byte_byffer(bytesRead*sizeof(_TChar));
		buff_.flip();
		conv_->convert(buff_,convBuff);
		return String(convBuff.begin(),convBuff.last());
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
	inline void write(const String& str) throw(io_exception) {
		byte_buffer srcBytes = new_byte_byffer(str.length()*sizeof(_TChar));
		const size_t destCharSize = conv_->destinationCharset()->charSize();
		std::copy(str.begin(),str.end(), srcBytes.begin());
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
