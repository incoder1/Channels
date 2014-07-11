#ifndef BYTE_BUFFER_HPP_INCLUDED
#define BYTE_BUFFER_HPP_INCLUDED

#include <stdint.h>
#include <vector>

#include <boost/function.hpp>
#include <buffers.hpp>

namespace io {

class byte_buffer_allocator;

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

	static byte_buffer new_heap_buffer(const std::size_t capacity) throw(std::bad_alloc);

	template<typename ElementType>
	static byte_buffer wrap_array(ElementType* const arr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW;

	template<typename ElementType>
	static byte_buffer copy_array(ElementType* const arr, std::size_t size) throw(std::bad_alloc);

	inline std::size_t put(uint8_t e) {
		return basic_buffer<uint8_t>::put(e);
	}

	std::size_t put(uint8_t* first, uint8_t* last) {
		std::size_t distance = std::size_t(last - first);
		std::size_t result = distance < remain() ? distance : remain();
		std::copy(first, last+result, position());
		move(result);
		return result;
	}

	std::size_t put(iterator& first, iterator& last) {
		return basic_buffer<uint8_t>::put(first, last);
	}

	std::size_t put(const_iterator& first,const_iterator& last) {
		return  basic_buffer<uint8_t>::put(first, last);
	}

};

/**
 * ! \brief Factory class for allocating a byte buffers.
 * <p>
 *  You can any kind of allocators for allocating byte buffers.
 *  The default behavior is simply new/delete allocators.
 * <\p>
 */
class byte_buffer_allocator {
public:
	/**
	 * Type of functor to be used to allocate memory
	 */
	typedef boost::function<uint8_t* (std::size_t)> alloc_functor_t;
	/**
	 * Type of functor to be used to deallocate memory
	 */
	typedef boost::function<void (uint8_t*)> free_functor_t;

	/**
	 * Constructing a new byte buffer allocator
	 * \param alloc_functor functor for allocating memory
	 * \param free_functor functor for free allocated memory
	 */
	byte_buffer_allocator(alloc_functor_t alloc_functor,free_functor_t free_functor) BOOST_NOEXCEPT_OR_NOTHROW:
		alloc_functor_(alloc_functor),
		free_functor_(free_functor)
	{}
	byte_buffer allocate(std::size_t capacity) const throw(std::bad_alloc) {
		uint8_t* data = alloc_functor_(capacity);
		uint8_t* const endp = data+capacity+1;
		boost::shared_array<uint8_t> array(data,free_functor_);
		return byte_buffer(array, endp);
	}
private:
	alloc_functor_t alloc_functor_;
	free_functor_t free_functor_;
};


template<typename ElementType>
byte_buffer byte_buffer::wrap_array(ElementType* const arr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
	empty_alloc<uint8_t> fake_allocator(reinterpret_cast<const uint8_t*>(arr));
	empty_free<uint8_t> fake_free;
	byte_buffer_allocator::alloc_functor_t alloc(fake_allocator);
	byte_buffer_allocator::free_functor_t free(fake_free);
	byte_buffer_allocator all(alloc,free);
	byte_buffer result = all.allocate(size);
	result.move(size);
	result.flip();
	return result;
}

template<typename ElementType>
byte_buffer byte_buffer::copy_array(ElementType* const arr, std::size_t size) throw(std::bad_alloc) {
	byte_buffer result = byte_buffer::new_heap_buffer(size);
	uint8_t* start = (uint8_t*)(arr);
	uint8_t* last = start+(size*sizeof(ElementType));
	std::copy(start,last,result.position());
	result.move(size);
	result.flip();
	return result;
}


} // namespace io

#endif // BYTE_BUFFER_HPP_INCLUDED
