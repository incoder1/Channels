#ifndef READER_HPP_INCLUDED
#define READER_HPP_INCLUDED

#include <stdexcept>
#include <charbuffers.hpp>
#include <convert.hpp>
#include <channels.hpp>

namespace io {

template<class CharBuff, class Converter>
class Reader
{
private:
	PReadChannel src_;
	byte_buffer buff_;
	Converter conv_;
public:
	Reader(PReadChannel src, const byte_buffer& buff,const Converter& conv) BOOST_NOEXCEPT:
		src_(src),
		buff_(buff),
		conv_(conv)
	{}
	inline size_t read(CharBuff& charBuff) throw(io_exception) {
		buff_.clear();
		size_t bytesRead = src_->read(buff_);
		buff_.flip();
		conv_.convert(buff_,charBuff);
		return charBuff.length();
	}
};

template<typename CharT>
Reader< char_buffer<CharT> , conveter<CharT> > create_reader(PReadChannel src,const conveter<CharT>& conv, size_t buffSize) throw(std::bad_alloc)
{
	byte_buffer buff = new_byte_byffer(buffSize);
	return Reader< char_buffer<CharT>, conveter<CharT> >(src, buff,conv);
}

} // namespace io

#endif // READER_HPP_INCLUDED
