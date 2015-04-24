#ifndef __IO_MEMORY_HPP_INCLUDED__
#define __IO_MEMORY_HPP_INCLUDED__

#include <windows.h>
#include <boost/atomic.hpp>
#include <boost/thread/once.hpp>

namespace io {

namespace detail {

class private_heap_block_allocator {
public:
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
private:
	private_heap_block_allocator();
	static void initialize();
	static void release();
	char* allocate(const std::size_t size);
	void dispose(char* const block);
	inline void lock();
public:
	static inline private_heap_block_allocator* const instance() {
		boost::call_once(&private_heap_block_allocator::initialize, _once);
		return _instance;
	}
	~private_heap_block_allocator() BOOST_NOEXCEPT_OR_NOTHROW;
	inline static char * malloc(const std::size_t bytes) {
		return instance()->allocate(bytes);
	}
	inline static void free(char * const block) {
		return instance()->dispose(block);
	}
private:
	static private_heap_block_allocator* _instance;
	static boost::once_flag _once;
	::HANDLE heap_;
	boost::atomics::atomic_size_t countdown_;
};


} // namespace detail

} // namespace io

#endif // __IO_MEMORY_HPP_INCLUDED__
