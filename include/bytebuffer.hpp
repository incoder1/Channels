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
	byte_buffer(boost::shared_array<uint8_t> data, uint8_t* const endp) BOOST_NOEXCEPT_OR_NOTHROW:
		basic_buffer<uint8_t>(data,endp)
	{}
	friend class byte_buffer_allocator;
public:
	typedef basic_buffer<uint8_t>::iterator iterator;
	typedef basic_buffer<uint8_t>::const_iterator const_iterator;

	byte_buffer(const byte_buffer& c) BOOST_NOEXCEPT_OR_NOTHROW:
		basic_buffer<uint8_t>( static_cast<basic_buffer<uint8_t> >(c))
	{}

	inline std::size_t put(uint8_t e) {
		return basic_buffer<uint8_t>::put(e);
	}

	std::size_t put(uint8_t* begin, uint8_t* end) {
		std::size_t result = (end - begin) > 0 ?  end - begin : 0;
		if(result) {
			for(uint8_t* i = begin; i < end; i++) {
				basic_buffer<uint8_t>::put(*i);
			}
		}
		return result;
	}

	std::size_t put(iterator& first, iterator& last) {
		return basic_buffer<uint8_t>::put(first, last);
	}

	std::size_t put(const_iterator& first,const_iterator& last) {
		return  basic_buffer<uint8_t>::put(first, last);
	}

	inline std::size_t put(const byte_buffer& another) {
		return basic_buffer<uint8_t>::put(another);
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
	/**
	 * Wraps buffer of any type into byte buffer
	 * \param buff
	 *			 buffer of any type
	 * \return byte buffer
	 */
	template<typename T>
	static byte_buffer wrap_buffer(const basic_buffer<T>& buff) {
		buff.flip();
		boost::shared_array<T> data = buff.ref();
		alloc_functor_t noDel(&byte_buffer_allocator::no_delete_free);
		uint8_t *start = reinterpret_cast<uint8_t*>((buff.position()).ptr());
		uint8_t *end = reinterpret_cast<uint8_t*>((buff.last()).ptr());
		boost::shared_array<uint8_t> array(start, noDel);
		return byte_buffer(array, end);
	}

private:
	static void no_delete_free(uint8_t* data)
	{}
	alloc_functor_t alloc_functor_;
	free_functor_t free_functor_;
};


inline byte_buffer new_byte_byffer(const std::size_t capacity) throw(std::bad_alloc)
{
	byte_buffer_allocator::alloc_functor_t alloc(new_alloc<uint8_t>);
	byte_buffer_allocator::free_functor_t free(delete_free<uint8_t>);
	byte_buffer_allocator all(alloc,free);
	return all.allocate(capacity);
}


inline byte_buffer wrap_array(const uint8_t* arr, std::size_t size) {
	empty_alloc<uint8_t> fake_allocator(arr);
	empty_free<uint8_t> fake_free;
	byte_buffer_allocator::alloc_functor_t alloc(fake_allocator);
	byte_buffer_allocator::free_functor_t free(fake_free);
	byte_buffer_allocator all(alloc,free);
	byte_buffer result = all.allocate(size);
	result.move(size);
	result.flip();
	return result;
}

inline byte_buffer copy_array(const uint8_t* arr, std::size_t size) {
	byte_buffer result = new_byte_byffer(size);
	std::copy(arr,arr+size,result.begin());
	return result;
}

template<typename _CharT>
inline byte_buffer wrap_string(const std::basic_string<_CharT>& str) {
	std::size_t size = str.length()*sizeof(_CharT);
	return wrap_array(
		// we can covert any array to byte array
		reinterpret_cast<const uint8_t*>(str.data()),
		size);
}

template<typename _CharT>
inline byte_buffer copy_string(const std::basic_string<_CharT>& str) {
	std::size_t size = str.length()*sizeof(_CharT);
	return copy_array(
					reinterpret_cast<const uint8_t>(str.data()),
					size
					);
}

} // namespace io

#endif // BYTE_BUFFER_HPP_INCLUDED
