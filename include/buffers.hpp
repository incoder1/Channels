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
class buffer_iterator: public std::iterator<std::bidirectional_iterator_tag, std::ptrdiff_t> {
private:
	mutable T* position_;
public:

	typedef std::bidirectional_iterator_tag iterator_category;
	typedef T  value_type;
	typedef T&  reference;
	typedef T*  pointer;

	buffer_iterator(T* const position) BOOST_NOEXCEPT_OR_NOTHROW:
		position_(position)
	{}

	inline buffer_iterator operator++() const {
		++position_;
		return *this;
	}

	inline const buffer_iterator& operator--() const {
		--position_;
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
		return buffer_iterator(position_+offset);
	}

	const buffer_iterator operator-(const difference_type& offset) const {
		return buffer_iterator(position_-offset);
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

	difference_type operator+(const buffer_iterator& lhs) const {
		return position_ + lhs.position_;
	}

	difference_type operator-(const buffer_iterator& lhs) const {
		return position_ - lhs.position_;
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
	      position_(data_.get()),
	      last_(position_),
	      end_(end)
	{}
public:
	typedef buffer_iterator<T> iterator;
	typedef const buffer_iterator<T> const_iterator;
public:

	/**
	 * Move this buffer position into offset. If offset is larger then buffer capacity,
	 * position pointer would be moved into the buffer last element.
	 * \param offset the offset to move position pointer
	 */
	std::ptrdiff_t move(std::size_t offset) {
		T *endPtr = position_+offset;
		std::size_t result = (endPtr <= end_) ? offset : remain();
		position_ += result;
		if(position_ > last_) {
			last_ = position_+1;
		}
		if(last_ >= end_) {
			last_ = end_;
		}
		return result;
	}

	/**
	 * Move this buffer position to the iterator position
	 * \param position - iterator where to move
	 */
	iterator move(iterator position) {
		position_ = position.ptr();
		if(position_ > last_) {
			last_ = position_+1;
		}
		return iterator(position_);
	}

	/**
	 * Returns remain elements possible to put
	 * \return remaining elements count
	 */
	std::ptrdiff_t remain() const {
		return size_t(end_ - position_);
	}

	/**
	 * Iterator to the current buffer position
	 * \return iterator to the current buffer position
	 */
	iterator position() {
		return iterator(position_);
	}

	/**
	 * Constant iterator to the current buffer position
	 * \return iterator to the current buffer position
	 */
	const_iterator position() const {
		return iterator(position_);
	}

	/**
	 * Iterator to the last filled buffer element
	 * \return constant iterator to the last buffer element
	 */
	iterator last() {
		return iterator(last_);
	}


	/**
	 * Constant iterator after the last filled buffer element
	 * \return constant iterator to the last buffer element
	 */
	const_iterator last() const {
		return const_iterator(last_);
	}

	const_iterator end() const {
		return const_iterator(end_);
	}

	/**
	 * Inserts a single element into last buffer, increases if success the position.
	 * \param t element to be inserted into current buffer position
	 * \return 1 if element was put, 0 if no more space left
	 */
	iterator put(T& e)  {
		if(position_ + 1 != end_) {
			*position_ = e;
			++position_;
			last_ = position_ + 1;
		}
		return position();
	}

	iterator put(iterator& first, iterator& last) {
		ssize_t size = ssize_t(last - first)+1;
		ssize_t offset = ((position_ + size) <= end_) ? size : remain();
		std::copy(first, first+offset, position_);
		position_ += offset;
		last_ = position_ + 1;
		return position();
	}

	iterator put(const_iterator& first,const_iterator& last) {
		return put(const_cast<iterator&>(first), const_cast<iterator&>(last));
	}


	/**
	 * Sets current buffer position into array begin,
	 * Use this method when you need read data from the buffer
	 */
	void flip() {
		// put last on the current state, and move position to the begin
		position_ = data_.get();
	}

	/**
	 * Set buffer position into array begin then nullify buffers length
	 */
	void clear() {
		position_ = data_.get();
		last_ = position_;
	}

	bool empty() const {
		return (last_ == position_);
	}

	bool full() const {
		return last_ == end_-1;
	}

	/**
	 * Count of filled elements
	 * \return buffer length
	 */
	std::ptrdiff_t length() const {
		return last_ - data_.get();
	}

	/**
	 * This buffer capacity
	 * \return buffer capacity
	 */
	std::ptrdiff_t capacity() const {
		return end_ - data_.get();
	}

private:
	boost::shared_array<T> data_;
	T* position_;
	T* last_;
	T* end_;
};


} // namespace io

#endif // BUFFERS_HPP_INCLUDED
