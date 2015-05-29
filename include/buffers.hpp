#ifndef BUFFERS_HPP_INCLUDED
#define BUFFERS_HPP_INCLUDED

#include <stdint.h>
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
	      last_(position_+1),
	      end_(end)
	{}
	void resize(T* data, T* const endp) {
		T* thisData = data_.get();
		position_ = data + (position_ - thisData);
		last_ = data + (last_ - thisData);
		std::copy(thisData,end_,data);
		end_ = endp;
		data_.reset(data);
	}
	void clone(basic_buffer& buff) {
		std::copy(data_.get(),end_,buff.data_.get());
		typedef typename iterator::difference_type diff_t;
		diff_t pos = position_ - data_.get();
		diff_t last = last_ - data_.get();
		buff.position_ = buff.data_.get() + pos;
		buff.last_ = buff.data_.get() + last;
	}
public:
	typedef buffer_iterator<T> iterator;
	typedef const buffer_iterator<T> const_iterator;
	typedef typename iterator::difference_type diff_t;
public:

	/**
	 * Move this buffer position into offset. If offset is larger then buffer capacity,
	 * position pointer would be moved into the buffer last element.
	 * \param offset the offset to move position pointer
	 */
	std::ptrdiff_t move(std::size_t offset) {
		iterator prevPos = position();
		return move(prevPos + offset) - prevPos;
	}

	/**
	 * Move this buffer position to the iterator position
	 * \param position - iterator where to move
	 */
	iterator move(iterator position) {
		position_ = position.ptr();
		if(position_ >= end_) {
			position_ = end_-1;
			last_ = end_;
		} else {
			last_ = (position_ > last_) ? position_+1 : last_;
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
	 * Iterator iterator to the next element after the last inserted element
	 * \return constant iterator to the last buffer element
	 */
	iterator last() {
		return iterator(last_);
	}


	/**
	 * Constant iterator to the next element after the last inserted element
	 * \return constant iterator to the last buffer element
	 */
	const_iterator last() const {
		return const_iterator(last_);
	}

	/**
	 * Returns constant iterator on first memory element of this buffer
	 */
	const_iterator begin() const {
		return const_iterator(data_.get());
	}

	/**
	 * Returns constant iterator on element after last element of this buffer
	 */
	const_iterator end() const {
		return const_iterator(end_);
	}

	/**
	 * Inserts a single element into last buffer, increases if success the position.
	 * \param t element to be inserted into current buffer position
	 * \return 1 if element was put, 0 if no more space left
	 */
	iterator put(T& e) {
		iterator pos = position();
		if(pos+1 < end() ) {
			*(++pos) = e;
		}
		return move(pos);
	}

	iterator put(iterator& first, iterator& last) {
		iterator dest = position();
		iterator border =  ( (last - first) < remain() ) ? last : first + remain();
		dest = std::copy(first, border, dest);
		return move(dest);
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
		return (last_ == position_-1);
	}

	bool full() const {
		return last_ == end_;
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

	/**
	 * Swaps this buffer with another
	 * \param oth buffer to swap with this buffer
	 */
	void swap(basic_buffer& oth) {
		data_.swap(oth.data_);
		std::swap(position_,oth.position_);
		std::swap(last_,oth.last_);
		std::swap(end_,oth.end_);
	}

private:
	boost::shared_array<T> data_;
	T* position_;
	T* last_;
	T* end_;
};

} // namespace io

#endif // BUFFERS_HPP_INCLUDED
