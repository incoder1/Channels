#ifndef BYTE_BUFFER_HPP_INCLUDED
#define BYTE_BUFFER_HPP_INCLUDED

#include <stdint.h>
#include <vector>

#include <boost/function.hpp>
#include <buffers.hpp>

namespace io {

/**
 * ! \brief  buffer implementation which operating bytes (uint8_t or unsigned char)
 */
class CHANNEL_PUBLIC byte_buffer:public basic_buffer<uint8_t> {
private:
	byte_buffer(boost::shared_array<uint8_t> data, uint8_t* const endp) BOOST_NOEXCEPT_OR_NOTHROW;
	friend class byte_buffer_allocator;
public:
	typedef basic_buffer<uint8_t>::iterator iterator;
	typedef basic_buffer<uint8_t>::const_iterator const_iterator;

	static byte_buffer heap_buffer(const std::size_t capacity) throw(std::bad_alloc);

	template<typename ElementType>
	static byte_buffer wrap_array(ElementType* const arr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW;

	template<typename ElementType>
	static byte_buffer copy_array(ElementType* const arr, std::size_t size) throw(std::bad_alloc);

	static byte_buffer wrap_str(const char* str) BOOST_NOEXCEPT_OR_NOTHROW;

	/**
	 *
	 */
	iterator put(uint8_t e);

	iterator put(uint8_t* const first, uint8_t* const last);

	iterator put(const byte_buffer& buff);

	iterator put(iterator& first, iterator& last);

	iterator put(const_iterator& first,const_iterator& last);
};


namespace _private {
	class empty_free {
	public:
		inline void operator()(uint8_t *)
		{}
	};
}


template<typename ElementType>
byte_buffer byte_buffer::wrap_array(ElementType* const arr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
	boost::shared_array<uint8_t> data((uint8_t*)arr, _private::empty_free() );
	ElementType *endp = arr + size;
	byte_buffer result(data,(uint8_t*)endp);
	result.move(result.end());
	result.flip();
	return result;
}

template<typename ElementType>
byte_buffer byte_buffer::copy_array(ElementType* const arr, std::size_t size) throw(std::bad_alloc) {
	std::size_t arraySize = size*sizeof(ElementType);
	byte_buffer result = byte_buffer::heap_buffer(arraySize);
	uint8_t* start = (uint8_t*)(arr);
	uint8_t* last = start+arraySize;
	result.put(start, last);
	result.flip();
	return result;
}


} // namespace io

#endif // BYTE_BUFFER_HPP_INCLUDED
