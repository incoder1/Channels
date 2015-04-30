#ifndef __IO_MEMORY_HPP_INCLUDED__
#define __IO_MEMORY_HPP_INCLUDED__

#include <windows.h>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>

namespace io {

namespace detail {

class private_heap_block_allocator {
public:
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
public:
	private_heap_block_allocator();
	~private_heap_block_allocator() BOOST_NOEXCEPT_OR_NOTHROW;
	static char* malloc(const std::size_t bytes);
	static void free(char * const block);
	static volatile private_heap_block_allocator* volatile instance();
private:
	static void release() BOOST_NOEXCEPT_OR_NOTHROW;
private:
	static boost::mutex _mutex;
	static volatile private_heap_block_allocator* volatile _instance;
	::HANDLE heap_;
};


} // namespace detail

} // namespace io

#endif // __IO_MEMORY_HPP_INCLUDED__
