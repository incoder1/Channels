#include "xmlsource.hpp"

#include <console.hpp>

namespace xml {

using namespace io;


Source::Source(SReadChannel source, SConverter charsetConverter,const byte_buffer& readBuff,const byte_buffer& convBuff) BOOST_NOEXCEPT_OR_NOTHROW:
	src_(source),
	converter_(charsetConverter),
	readBuff_(readBuff),
	convBuff_(convBuff)
{}


std::size_t Source::readMore() {
	readBuff_.clear();
	convBuff_.clear();
	std::size_t result = src_->read(readBuff_);
	if( result > 0 ) {
		readBuff_.flip();
		result = converter_->convert( readBuff_, convBuff_);
	}
	return result;
}

static inline bool need_more_data(const byte_buffer& buff) {
	return buff.empty() || (buff.position()+1 == buff.end());
}

bool Source::hasNext() {
	bool result = need_more_data(convBuff_);
	result = result ? readMore() > 0 : !result;
	return result;
}

uint8_t Source::nextByte() {
	io::byte_buffer::iterator pos = convBuff_.position();
	uint8_t result = *pos;
	convBuff_.move(++pos);
	return result;
}

Source Source::create(io::SReadChannel source) {
	byte_buffer readB = byte_buffer::heap_buffer(128);
	byte_buffer convB = byte_buffer::heap_buffer(128);
	SConverter converter = char_empty_converter();
	return Source(source,converter,readB,convB);
}

Source Source::create(io::SReadChannel src, const char* inputChaset) {
	std::string inch(inputChaset);
	if(inch == std::string("UTF-8")) {
		return Source::create(src);
	}
	io::SConverter conv = io::new_converter(inputChaset,"UTF-8");
	byte_buffer read = byte_buffer::heap_buffer(128);
	byte_buffer cnvb = byte_buffer::heap_buffer(512);
	return Source(src,conv,read,cnvb);
}

} // namespace xml
