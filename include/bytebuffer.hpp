#ifndef BYTE_BUFFER_HPP_INCLUDED
#define BYTE_BUFFER_HPP_INCLUDED

#include <stdint.h>
#include <vector>

#include <boost/function.hpp>

#include "channels_config.h"
#include "buffers.hpp"

namespace io {

/**
 * ! \brief a buffer implementation which operating bytes (uint8_t or unsigned char) as element type
 */
class CHANNEL_PUBLIC byte_buffer:public basic_buffer<uint8_t> {
private:
	byte_buffer(boost::shared_array<uint8_t> data, uint8_t* const endp) BOOST_NOEXCEPT_OR_NOTHROW;
public:
	typedef basic_buffer<uint8_t>::iterator iterator;
	typedef basic_buffer<uint8_t>::const_iterator const_iterator;

	/**
	 * Creates a new byte buffer with dynamical allocating memory for the content
	 * \param capacity the size in bytes for buffer content
	 * \throw std::bad_alloc if allocation of dynamical memory has been failed
	 */
	static byte_buffer heap_buffer(const std::size_t capacity) throw(std::bad_alloc);

	/**
	 * Wraps an C style array into byte buffer
	 *
	 * Please note, if underlying wrap memory is not controlled by byte_buffer.
	 * Use wrapping only in scope of the current context.
	 * <pre>
	 * Valid usage:
	 *	uint8_t data[3] = {0x00,0xFA,0xFF};
     *	io::byte_buffer buff = io::byte_buffer::wrap_array(data, 3);
     *	doSomething(buff); // data array still in stack
     *
	 *	Invalid usage:
	 *	io::byte_buffer get_buffer() {
	 * 		uint8_t data[3] = {0x00,0xFA,0xFF};
     *		return io::byte_buffer::wrap_array(data, 3);
	 *	}
	 * ....
	 * // array lives stack, buffer pointer shows on invalid memory block
	 *	doSomething(get_buffer());
	 * </pre>
	 *
	 * \tparam ElementType the type of C style array
	 * \param arr a pointer on memory block to start copy
	 * \param size count of array element to copy
	 * \return the wrap reference buffer C array wrapped memory content
	 */
	template<typename ElementType>
	static byte_buffer wrap_array(ElementType* const arr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW;

	/**
	 * Deep copy an C style array into byte buffer
	 * \tparam ElementType the type of C style array
	 * \param arr a pointer on memory block to start copy
	 * \param size count of element to copy
	 * \return the byte buffer with deep copied memory content
	 */
	template<typename ElementType>
	static byte_buffer copy_array(ElementType* const arr, std::size_t size) throw(std::bad_alloc);

	/**
	 * Puts single byte into buffer
	 * \param e byte element to put into buffer
	 * \return iterator to the new buffer position, or on the previews position if buffer is full
	 */
	iterator put(uint8_t e);

	/**
	 * Puts range of continues memory bytes into this buffer by deep copying it
	 * \param first pointer on first byte of continues memory range
	 * \param last pointer on last byte of continues memory range
	 * \return iterator to the new buffer position, or on the previews position if buffer is full
	 */
	iterator put(uint8_t* const first, uint8_t* const last);
	/**
	 * Puts a byte buffer into this buffer by deep copying it, starting from position ending by last
	 * \param buff a buffer to put
	 * \return iterator to the new buffer position, or on the previews position if buffer is full
	 */
	iterator put(const byte_buffer& buff);

	/**
	 * Puts content of another buffer into this buffer by deep copying it
	 * \param first buffer iterator to start copying
	 * \param last the last iterator to stop copying
	 * \return iterator to the new buffer position, or on the previews position if buffer is full
	 */
	iterator put(iterator& first, iterator& last);

	/**
	 * Puts content of another buffer into this buffer by deep copying it
	 * \param first constant buffer iterator to start copying
	 * \param last the last constant iterator to stop copying
	 * \return iterator to the new buffer position, or on the previews position if buffer is full
	 */
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
	boost::shared_array<uint8_t> data((uint8_t*)arr, _private::empty_free());
	ElementType *endp = arr + size;
	byte_buffer result(data,(uint8_t*)endp);
	result.move(result.end());
	result.flip();
	return result;
}

template<typename ElementType>
byte_buffer byte_buffer::copy_array(ElementType* const arr, std::size_t size) throw(std::bad_alloc)
{
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
