#ifndef BUFFERS_HPP_INCLUDED
#define BUFFERS_HPP_INCLUDED

#include <cstddef>
#include <memory>
#include <algorithm>
#include <string>

#include <cstdio>

#include <boost/limits.hpp>
#include <boost/noncopyable.hpp>
#include <boost/utility.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>


namespace io {

/**
 * ! \brief The buffer iterator template,
 * Random access iterator, STL compatible
 * The ++ and -- operators are cyclic
 */
template<typename T>
class buffer_iterator {
private:
	T* const begin_;
	T* const end_;
	mutable T* position_;
public:

	typedef std::random_access_iterator_tag iterator_category;
	typedef T  value_type;
	typedef size_t difference_type;
	typedef T&  reference;
	typedef T*  pointer;

explicit buffer_iterator(T* begin, T* const end, T* const position) BOOST_NOEXCEPT:
	begin_(begin),
	       end_(end),
	       position_(position)
	{}

	inline bool operator==(const buffer_iterator& it) const {
		return position_ == it.position_;
	}

	inline bool operator!=(const buffer_iterator& it) const {
		return position_ != it.position_;
	}

	inline difference_type operator-(const buffer_iterator& it) const {
		return position_ - it.position_;
	}

	inline buffer_iterator operator++() const {
		if(end_ == (position_ + 1) ) {
			return buffer_iterator(begin_, end_, end_);
		} else {
			++position_;
		}
		return *this;
	}

	inline const buffer_iterator& operator+(difference_type step) const {
		if((position_ + step) >= end_) {
			position_ = end_;
		} else {
			position_ += step;
		}
		return *this;
	}

	inline const buffer_iterator& operator--() const {
		if(begin_ == (position_ - 1) ) {
			position_ = end_-1;
		} else {
			--position_;
		}
		return *this;
	}

	inline reference operator*() const {
		return *position_;
	}

	inline pointer operator->() const {
		return position_;
	}

	inline pointer const ptr() const {
		return position_;
	}
};

/**
 * ! \brief basic buffer template. Buffers is a wrappers on top of array
 * Unlike vectors  buffers have a fixed size i.e. they can't grow.
 * Buffers also works like shared arrays
 * There is no [] operator or get function. Iterators are used instead
 * Data array allocated by allocator provided by template parameter.
 */
template<typename T>
class basic_buffer {
private:
	boost::shared_array<T> data_;
	T* position_;
	T* last_;
	T* end_;
protected:
explicit basic_buffer(boost::shared_array<T> data, T* const end) BOOST_NOEXCEPT:
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
	inline const_iterator begin() const {
		return const_iterator(NULL, NULL, data_.get());
	}

	/**
	 * Constant iterator to the after last buffer element
	 * \return constant end iterator
	 */
	inline const_iterator end() const {
		return const_iterator(NULL, NULL, end_);
	}

	/**
	 * Iterator to the before first buffer element
	 * \return begin iterator
	 */
	inline iterator begin() {
		return iterator(NULL, NULL, data_.get());
	}

	/**
	 * Move this buffer position into offset. If offset is larger then buffer capacity,
	 * position pointer would be moved into the buffer last element.
	 * \param offset the offset to move position pointer
	 */
	void move(size_t offset) {
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
	 * Returns how count of element possible to put into buffer
	 */
	size_t free() const {
		return (end_-1) - last_;
	}

	/**
	 * Iterator to the after last buffer element
	 * \return begin iterator
	 */
	inline iterator end() {
		return iterator(NULL, NULL , end_);
	}

	/**
	 * Iterator to the current buffer position
	 * \return iterator to the current buffer position
	 */
	inline iterator position() {
		return iterator(data_.get()-1, end_, position_);
	}

	/**
	 * Constant iterator to the current buffer position
	 * \return iterator to the current buffer position
	 */
	inline const_iterator position() const {
		return iterator(data_.get()-1, end_, position_);
	}

	/**
	 * Iterator to the last buffer element
	 * \return constant iterator to the last buffer element
	 */
	inline iterator last() {
		return const_iterator(data_.get()-1, end_, last_);
	}


	/**
	 * Constant iterator to the last buffer element
	 * \return constant iterator to the last buffer element
	 */
	inline const_iterator last() const {
		return const_iterator(NULL, end_, last_);
	}

	/**
	 * Inserts a single element into the buffer end.
	 * This method incrementing buffers length
	 * \param t element to be inserted into current buffer position
	 */
	inline size_t put(T& e)  {
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

	template<class iterator_t>
	size_t put(const iterator_t& b,iterator_t& e) {
		size_t avaliable = capacity() - length();
		size_t count = static_cast<size_t>(e - b);
		size_t offset = (count > avaliable) ? avaliable : count;
		std::copy(b,b+offset,position_);
		if( (last_ == position_) ) {
			last_ += offset;
		}
		position_ += offset;
		return offset;
	}

	inline size_t put(const T* arr, size_t size) {
		size_t avaliable = capacity() - length();
		size_t offset = (size > avaliable) ? avaliable : size;
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
	inline void flip() {
		position_ = data_.get();
	}

	/**
	 * Set buffer position into array begin then nullify buffers length
	 */
	inline void clear() {
		flip();
		last_ = data_.get();
	}

	/**
	 * This buffer length
	 * \return buffer length
	 */
	inline size_t length() const {
		return last_ - data_.get();
	}

	/**
	 * This buffer capacity
	 * \return buffer capacity
	 */
	inline size_t capacity() const {
		return (end_-1) - data_.get();
	}

	/**
	 * Deallocates buffer array
	 */
	virtual ~basic_buffer() = 0;
};

template<typename T>
basic_buffer<T>::~basic_buffer()
{
}

template<typename T>
inline T* new_alloc(const size_t count) throw(std::bad_alloc)
{
	return new T[count];
}

template<typename T>
inline void delete_free(T* ptr)
{
	delete [] ptr;
}

} // namespace io

#endif // BUFFERS_HPP_INCLUDED
