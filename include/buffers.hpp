#ifndef BUFFERS_HPP_INCLUDED
#define BUFFERS_HPP_INCLUDED

#include <cstddef>
#include <algorithm>

#include <boost/limits.hpp>
#include <boost/utility.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/array.hpp>

namespace io {

/**
 * ! \brief The buffer iterator template,
 * Random access iterator, STL compatible
 */
template<typename T>
class buffer_iterator: public std::iterator<std::bidirectional_iterator_tag,uint64_t> {
private:
	T* const begin_;
	T* const end_;
	mutable T* position_;
public:

	typedef std::bidirectional_iterator_tag iterator_category;
	typedef T  value_type;
	typedef T&  reference;
	typedef T*  pointer;

	buffer_iterator(T* begin, T* const end, T* const position) BOOST_NOEXCEPT_OR_NOTHROW:
		   begin_(begin),
	       end_(end),
	       position_(position)
	{}

	buffer_iterator(const buffer_iterator& c) BOOST_NOEXCEPT_OR_NOTHROW:
		begin_(c.begin_),
		end_(c.end_),
		position_(c.position_)
	{}

	buffer_iterator& operator=(const buffer_iterator& c) BOOST_NOEXCEPT_OR_NOTHROW
	{
		buffer_iterator cp(c);
		std::swap(*this,cp);
		return *this;
	}

	inline buffer_iterator operator++() const {
		position_ = (end_ ==  position_) ? end_: position_ + 1;
		return *this;
	}

	inline const buffer_iterator& operator--() const {
		position_ = (begin_ == position_) ? begin_ : position_ - 1;
		return *this;
	}

	reference operator*() const {
		return *position_;
	}

	pointer operator->() const {
		return position_;
	}

	inline pointer const ptr() const {
		return position_;
	}

	const buffer_iterator operator+(const difference_type& offset) const {
		return buffer_iterator(begin_, end_, position_+offset);
	}

	const buffer_iterator operator-(const difference_type& offset) const {
		return buffer_iterator(begin_, end_, position_-offset);
	}

	inline bool operator==(const buffer_iterator& it) const {
		return position_ == it.position_;
	}

	inline bool operator!=(const buffer_iterator& it) const {
		return position_ != it.position_;
	}

	bool operator<(const buffer_iterator& rhs) const {
		return position_ < rhs.position_;
	}

	bool operator>(const buffer_iterator& rhs) const {
		return position_ > rhs.position_;
	}

	bool operator>=(const buffer_iterator& rhs) const {
		return position_ >= rhs.position_;
	}

	bool operator<=(const buffer_iterator& rhs) const {
		return position_ <= rhs.position_;
	}
};

/**
 * ! \brief Basic buffer template. Buffers is a wrappers on top of array
 * Unlike vectors  buffers have a fixed size i.e. they can't grow.
 * There is no [] operator or get function. Iterators are used instead
 * Array data type allocated by allocator provided by template parameter.
 */
template<typename T>
class basic_buffer {
protected:
	basic_buffer(boost::shared_array<T> data, T* const end) BOOST_NOEXCEPT_OR_NOTHROW:
		  data_(data),
	      position_(const_cast<T*>(data_.get())),
	      last_(position_),
	      end_(end)
	{}
public:
	typedef buffer_iterator<T> iterator;
	typedef const buffer_iterator<T> const_iterator;
public:

	/**
	 * Constant iterator to the before first buffer element
	 * \return constant begin iterator
	 */
	const_iterator begin() const {
		return const_iterator(NULL, NULL, data_.get());
	}

	/**
	 * Constant iterator to the after last buffer element
	 * \return constant end iterator
	 */
	const_iterator end() const {
		return const_iterator(NULL, NULL, end_);
	}

	/**
	 * Iterator to the before first buffer element
	 * \return begin iterator
	 */
	iterator begin() {
		return iterator(NULL, NULL, data_.get());
	}

	/**
	 * Move this buffer position into offset. If offset is larger then buffer capacity,
	 * position pointer would be moved into the buffer last element.
	 * \param offset the offset to move position pointer
	 */
	void move(std::size_t offset) {
		if( (position_+offset) >= end_ ) {
			last_ = end_ - 1;
			position_ = last_;
		} else {
			if( (position_ + offset) >= last_) {
				last_ = position_ + offset;
			}
			position_ += offset;
		}
	}

	/**
	 * Returns remain elements possible to put
	 * \return remaining elements count
	 */
	std::size_t remain() const {
		return (end_-1) - last_;
	}

	/**
	 * Iterator to the after last buffer element
	 * \return begin iterator
	 */
	iterator end() {
		return iterator(NULL, NULL , end_);
	}

	/**
	 * Iterator to the current buffer position
	 * \return iterator to the current buffer position
	 */
	iterator position() {
		return iterator(data_.get(), end_, position_);
	}

	/**
	 * Constant iterator to the current buffer position
	 * \return iterator to the current buffer position
	 */
	const_iterator position() const {
		return iterator(data_.get(), end_, position_);
	}

	/**
	 * Iterator to the last buffer element
	 * \return constant iterator to the last buffer element
	 */
	iterator last() {
		return const_iterator(NULL, end_, last_);
	}


	/**
	 * Constant iterator to the last buffer element
	 * \return constant iterator to the last buffer element
	 */
	const_iterator last() const {
		return const_iterator(NULL, end_, last_);
	}

	/**
	 * Inserts a single element into the buffer end.
	 * This method incrementing buffers length
	 * \param t element to be inserted into current buffer position
	 */
	std::size_t put(T& e)  {
		if(position_ + 1 != end_) {
			*position_ = e;
			if( (last_ == position_) || (position_+ 1 > last_) ) {
				++position_;
				last_ = position_;
			} else {
				++position_;
			}
			return 1;
		}
		return 0;
	}

	std::size_t put(iterator& first, iterator& last) {
		std::size_t result = 0;
		for(iterator it = first; (it < last) && it <= end(); ++it) {
			put(*it);
			++result;
		}
		return result;
	}

	std::size_t put(const_iterator& first,const_iterator& last) {
		return put(const_cast<iterator&>(first), const_cast<iterator&>(last));
	}

	std::size_t put(const basic_buffer& other) {
		return put(other.position(), other.last());
	}

	std::size_t put(const T* arr, std::size_t size) {
		std::size_t avaliable = capacity() - length();
		std::size_t offset = (size > avaliable) ? avaliable : size;
		std::copy(arr, arr+offset, position_);
		if( (last_ == position_) ) {
			last_ += offset;
		}
		position_ += offset;
		return offset;
	}

	/**
	 * Sets current buffer position into array begin,
	 * Use this method when you need read data from the buffer
	 */
	void flip() {
		position_ = data_.get();
	}

	/**
	 * Set buffer position into array begin then nullify buffers length
	 */
	void clear() {
		position_ = data_.get();
		last_ = position_;
	}

	/**
	 * This buffer length
	 * \return buffer length
	 */
	std::size_t length() const {
		return last_ - data_.get();
	}

	/**
	 * This buffer capacity
	 * \return buffer capacity
	 */
	std::size_t capacity() const {
		return (end_-1) - data_.get();
	}

	basic_buffer(const basic_buffer& c) BOOST_NOEXCEPT_OR_NOTHROW:
		data_(c.data_),
		position_(c.position_),
		last_(c.last_),
		end_(c.end_)
	{}

	basic_buffer& operator=(const basic_buffer& rhs) BOOST_NOEXCEPT_OR_NOTHROW
	{
		basic_buffer cp(rhs);
		std::swap(*this,cp);
		return *this;
	}

	/**
	 * Deallocates buffer array
	 */
	virtual ~basic_buffer()
	{}
private:
	boost::shared_array<T> data_;
	T* position_;
	T* last_;
	T* end_;
};

/// Really allocates dynamic memory
// TODO: See whether can be replaces with the OS call for memory allocation
// instead of c++ stdlib based new operator
template<typename T>
inline T* new_alloc(const std::size_t count) throw(std::bad_alloc)
{
	return new T[count];
}

/// Deallocates block of dynamically allocated memory
template<typename T>
inline void delete_free(T* ptr)
{
	delete [] ptr;
}

// Do not allocate dynamic memory, used for wrapping static arrays or STL containers
template<typename T>
class empty_alloc {
private:
	const T* array_;
public:
	empty_alloc(const T* array) BOOST_NOEXCEPT_OR_NOTHROW:
		array_(array)
	{}
	inline T* operator()(std::size_t size) {
		return const_cast<T*>(array_);
	}
};

template<typename T>
class empty_free {
public:
	// should be removed by the compiler optimization
	inline void operator()(T*) BOOST_NOEXCEPT_OR_NOTHROW
	{}
};

} // namespace io

#endif // BUFFERS_HPP_INCLUDED
