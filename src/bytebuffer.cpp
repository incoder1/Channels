#include "prchdrs.h"
#include "bytebuffer.hpp"

namespace io {


byte_buffer::byte_buffer(boost::shared_array<uint8_t> data, uint8_t* const endp) BOOST_NOEXCEPT_OR_NOTHROW:
	basic_buffer<uint8_t>(data,endp)
{}


byte_buffer::iterator byte_buffer::put(uint8_t e)
{
	return basic_buffer<uint8_t>::put(e);
}

byte_buffer::iterator byte_buffer::put(const byte_buffer& buff)
{
	return put(buff.position(), buff.last());
}

byte_buffer::iterator byte_buffer::put(uint8_t* const first, uint8_t* const last)
{
	std::size_t distance = std::size_t(last - first);
	std::size_t result = distance < static_cast<std::size_t>(remain()) ? distance : static_cast<std::size_t>(remain());
	std::copy(first, first+result, position());
	move(result);
	return position();
}

byte_buffer::iterator byte_buffer::put(iterator& first, iterator& last)
{
	return basic_buffer<uint8_t>::put(first, last);
}

byte_buffer::iterator byte_buffer::put(const_iterator& first,const_iterator& last)
{
	return  basic_buffer<uint8_t>::put(first, last);
}

static uint8_t* new_heap_block(std::size_t size) throw (std::bad_alloc)
{
	return new uint8_t[size];
}

static void free_heap_block(uint8_t* block)
{
	delete [] block;
}

byte_buffer byte_buffer::heap_buffer(const std::size_t capacity) throw(std::bad_alloc)
{
	uint8_t* start = new_heap_block(capacity);
	boost::shared_array<uint8_t> data(start, free_heap_block);
	byte_buffer result(data, start+capacity);
	return result;
}

} //namaspace io
