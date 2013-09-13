#ifndef BYTE_BUFFER_HPP_INCLUDED
#define BYTE_BUFFER_HPP_INCLUDED

#include <stdint.h>
#include <vector>
#include <boost/function.hpp>

#include "buffers.hpp"

namespace io {

class byte_buffer_allocator;

class byte_buffer:public basic_buffer<uint8_t> {
private:
	byte_buffer(boost::shared_array<uint8_t> data, uint8_t* const endp) BOOST_NOEXCEPT:
		basic_buffer<uint8_t>(data,endp)
	{}
	friend class byte_buffer_allocator;
public:

	inline size_t put(uint8_t e) {
		return basic_buffer<uint8_t>::put(e);
	}

	inline size_t put(const uint8_t* arr, size_t size) {
		return basic_buffer<uint8_t>::put(arr, size);
	}

	inline size_t put(uint8_t* begin, uint8_t* end) {
		return basic_buffer<uint8_t>::put(begin, end);
	}

	inline size_t put(const std::vector<uint8_t>& v)
	{
		std::vector<uint8_t>::const_iterator b = v.begin();
		std::vector<uint8_t>::const_iterator e = v.end();
		return basic_buffer<uint8_t>::put(b,e);
	}

	template<class CharBuff>
	inline size_t put(const CharBuff& charBuff) {
		uint8_t *b = reinterpret_cast<uint8_t*>((charBuff.position()).ptr());
		uint8_t *e = reinterpret_cast<uint8_t*>((charBuff.last()).ptr());
		return basic_buffer<uint8_t>::put(b,e);
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
	typedef boost::function<uint8_t* (size_t)> alloc_functor_t;
	/**
	 * Type of functor to be used to deallocate memory
	 */
	typedef boost::function<void (uint8_t*)> free_functor_t;

	/**
	 * Constructing a new byte buffer allocator
	 * \param alloc_functor functor for allocating memory
	 * \param free_functor functor for free allocated memory
	 */
	byte_buffer_allocator(alloc_functor_t alloc_functor,free_functor_t free_functor) BOOST_NOEXCEPT:
			alloc_functor_(alloc_functor),
			free_functor_(free_functor)
	{}
	byte_buffer allocate(size_t capacity) const throw(std::bad_alloc) {
		uint8_t* data = alloc_functor_(capacity);
		uint8_t* const endp = data+capacity+1;
		boost::shared_array<uint8_t> array(data,free_functor_);
		return byte_buffer(array, endp);
	}
private:
	alloc_functor_t alloc_functor_;
	free_functor_t free_functor_;
};


inline byte_buffer new_byte_byffer(size_t capacity) throw(std::bad_alloc)
{
	byte_buffer_allocator::alloc_functor_t alloc(new_alloc<uint8_t>);
	byte_buffer_allocator::free_functor_t free(delete_free<uint8_t>);
	byte_buffer_allocator all(alloc,free);
	return all.allocate(capacity);
}

} // namespace io

#endif // BYTE_BUFFER_HPP_INCLUDED
