#ifndef CHAR_BUFFERS_HPP_INCLUDED
#define CHAR_BUFFERS_HPP_INCLUDED

#include "buffers.hpp"

namespace io {

template<class T>
class char_buffer_allocator;

/**
 * ! \brief Extension of the basic buffer temple for working with characters
 */
template<typename T>
class char_buffer:public basic_buffer<T> {
public:
	typedef T char_t;
	typedef std::basic_string<T> stl_str_t;
	typedef std::char_traits<T> char_traits_t;
private:
	typedef typename stl_str_t::const_iterator stl_str_c_it_t;

	char_buffer(boost::shared_array<T> data, T* const endp):
		basic_buffer<T>(data, endp)
	{}
	friend class char_buffer_allocator<T>;
public:
	void put(const T& t) {
		basic_buffer<T>::put(t);
	}

	/**
	 * Puts STL string into this buffer
	 * \param str STL string to put
	 */
	void put(const stl_str_t& str) {
		stl_str_c_it_t b = str.begin();
		stl_str_c_it_t e = str.end();
		basic_buffer<T>::put(b, e);
	}

	/**
	 * Puts C string into this buffer
	 * \param str C string to put
	 */
	void put(const T* str) {
		size_t length = char_traits_t::length(str);
		T* b = const_cast<T*>(str);
		T* e = const_cast<T*>(str+length);
		put(b,e);
	}

	void put(const T* begin, const T* end) {
		basic_buffer<T>::put(begin, end);
	}

	/**
	 * Deep copy this buffer characters starting from buffer begin ending with the last element
	 * into STL string
	 * \return STL string with this buffer data appended
	 */
	stl_str_t to_str() const {
		return stl_str_t(basic_buffer<T>::begin(), basic_buffer<T>::last());
	}
};

/**
 * ! \brief char_buffer_allocator factory template for allocating a character buffers
 */
template<class _CharT>
class char_buffer_allocator {
public:
	/**
	 * Type of functor to be used to allocate memory
	 */
	typedef boost::function<_CharT* (size_t)> alloc_functor_t;
	/**
	 * Type of functor to be used to deallocate memory
	 */
	typedef boost::function<void (_CharT*)> free_functor_t;
	/**
	 * Type of resulting character buffer
	 */
	typedef char_buffer<_CharT> char_buffer_t;
	/**
	 * Type of STL string type to be used for wrap operations
	 */
	typedef typename char_buffer_t::stl_str_t stl_string_t;
public:
	/**
	 * Constructing a new character buffer allocator
	 * \param alloc_functor functor for allocating memory
	 * \param free_functor functor for free allocated memory
	 */
char_buffer_allocator(const alloc_functor_t& alloc_functor, free_functor_t free_functor) BOOST_NOEXCEPT:
	alloc_functor_(alloc_functor),
	               free_functor_(free_functor)
	{}
	/**
	 * Allocates new character buffer
	 * \param capacity - the buffer capacity
	 * \return newly-created character buffer
	 */
	char_buffer_t allocate(size_t capacity) const throw(std::bad_alloc) {
		_CharT* data = alloc_functor_(capacity);
		_CharT* const endp = data+capacity+1;
		boost::shared_array<_CharT> array(data,free_functor_);
		return char_buffer_t(array, endp);
	}
	/**
	 * Wraps STL string into character buffer. The new buffer capacity will be the same as
	 * STL string length. Buffer position will be set into the buffers end.
	 * All STL string character data will be deep copied into buffer.
	 * \param str STL string to be wrapped
	 * \return newly-created character buffer
	 */
	char_buffer_t wrap(const stl_string_t& str) throw(std::bad_alloc) {
		char_buffer_t result = allocate(str.length());
		result.put(str);
		return result;
	}
	/**
	 * Wraps C-like zero ending pointer to string into character buffer.
	 * The new buffer capacity will be the same as C string.
	 * Buffer position will be set into the buffers end.
	 * All C string character data will be deep copied into buffer.
	 * \param str C-like zero ending pointer to string
	 * \return newly-created character buffer
	 */
	char_buffer_t wrap(const _CharT * str) throw(std::bad_alloc) {
		size_t length = char_buffer_t::char_traits_t::length(str);
		char_buffer_t result = allocate(length);
		result.put(str);
		return result;
	}
private:
	alloc_functor_t alloc_functor_;
	free_functor_t free_functor_;
};

/**
 * Template helper function for wrapping C like zero ending string into character buffer
 * This helper uses a default new/delete operations for allocating/free memory.
 * \param str C like zero ending string
 */
template<typename _CharT>
inline char_buffer<_CharT> wrap_cstr(const _CharT* str) throw(std::bad_alloc)
{
	boost::function<_CharT* (size_t) throw(std::bad_alloc)> alloc(new_alloc<_CharT>);
	boost::function<void (_CharT*) throw()> free(delete_free<_CharT>);
	char_buffer_allocator<_CharT> all(alloc,free);
	return all.wrap(str);
}

/**
 * Template helper function for wrapping STL string into character buffer
 * This helper uses a default new/delete operations for allocating/free memory.
 * \param str C like zero ending string
 */
template<typename _CharT>
inline char_buffer<_CharT> wrap_stl_str(const std::basic_string<_CharT>& str) throw(std::bad_alloc)
{
	boost::function<_CharT* (size_t) throw(std::bad_alloc)> alloc(new_alloc<_CharT>);
	boost::function<void (_CharT*) throw()> free(delete_free<_CharT>);
	char_buffer_allocator<_CharT> all(alloc,free);
	return all.wrap(str);
}

template<typename _CharT>
inline char_buffer<_CharT> new_char_buff(size_t capacity) throw(std::bad_alloc)
{
	boost::function<_CharT* (size_t)> alloc(new_alloc<_CharT>);
	boost::function<void (_CharT*)> free(delete_free<_CharT>);
	char_buffer_allocator<_CharT> all(alloc,free);
	return all.allocate(capacity);
}

typedef char_buffer<char> char_buffer_a;
typedef char_buffer_allocator<char> char_buffer_a_allocator;

// should be compliant with the C++03X
typedef char_buffer<wchar_t> char_buff_utf16;


} // namespace io

#endif // CHAR_BUFFERS_HPP_INCLUDED
