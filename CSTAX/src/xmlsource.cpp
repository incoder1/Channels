#include "xmlsource.hpp"

#include <console.hpp>

namespace xml {

using namespace io;

// helpers
inline bool need_more_data(const byte_buffer& buff) {
	return buff.empty() || (buff.position()+1 == buff.end());
}

// Source
Source::Source() BOOST_NOEXCEPT_OR_NOTHROW:
	io::object()
{}

Source::~Source() BOOST_NOEXCEPT_OR_NOTHROW {
}

// SimpleSource
SimpleSource::SimpleSource(io::SReadChannel data,const io::byte_buffer& readBuf) BOOST_NOEXCEPT_OR_NOTHROW:
	Source(),
	src_(data),
	buff_(readBuf)
{
	buff_.clear();
}

SimpleSource::~SimpleSource() BOOST_NOEXCEPT_OR_NOTHROW
{
}

std::size_t SimpleSource::readMore() {
	buff_.clear();
	std::size_t result = src_->read(buff_);
	if( result > 0 ) {
		buff_.flip();
	}
	return result;
}

bool SimpleSource::hasNext()
{
	bool result = need_more_data(buff_);
	result = result ? readMore() > 0 : !result;
	return result;
}

uint8_t SimpleSource::nextByte() {
	io::byte_buffer::iterator pos = buff_.position();
	uint8_t result = *pos;
	buff_.move(++pos);
	return result;
}

// Converting source
ConvertingSource::ConvertingSource(io::SReadChannel data,io::SConverter converter,const io::byte_buffer& readBuff):
	SimpleSource(data,readBuff),
	converter_(converter),
	convBuff_(byte_buffer::heap_buffer(readBuff.capacity()*2))
{
}

ConvertingSource::~ConvertingSource() BOOST_NOEXCEPT_OR_NOTHROW
{
}


std::size_t ConvertingSource::readMore() {
	convBuff_.clear();
	std::size_t result = SimpleSource::readMore();
	if( result > 0 ) {
		result = converter_->convert(readBuff(), convBuff_);
	}
	return result;
}

bool ConvertingSource::hasNext() {
	bool result = need_more_data(convBuff_);
	result = result ? readMore() > 0 : !result;
	return result;
}

uint8_t ConvertingSource::nextByte() {
	io::byte_buffer::iterator pos = convBuff_.position();
	uint8_t result = *pos;
	convBuff_.move(++pos);
	return result;
}


} // namespace xml
