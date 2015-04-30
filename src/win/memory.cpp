#include "prchdrs.h"
#include "memory.hpp"

#include <boost/pool/singleton_pool.hpp>


namespace io {

namespace detail {

// private_heap_block_allocator
boost::mutex private_heap_block_allocator::_mutex;
volatile private_heap_block_allocator* volatile private_heap_block_allocator::_instance = NULL;

volatile private_heap_block_allocator* volatile private_heap_block_allocator::instance()
{
	if(NULL == _instance) {
		boost::unique_lock<boost::mutex> lock(_mutex);
		if(NULL == _instance) {
			_instance = new volatile private_heap_block_allocator();
			std::atexit(&private_heap_block_allocator::release);
		}
	}
	return _instance;
}

void private_heap_block_allocator::release() BOOST_NOEXCEPT_OR_NOTHROW
{
	delete _instance;
}

private_heap_block_allocator::private_heap_block_allocator():
	heap_(NULL)
{
	heap_ = ::HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
}

private_heap_block_allocator::~private_heap_block_allocator() BOOST_NOEXCEPT_OR_NOTHROW
{
	::HeapDestroy(heap_);
}

char * private_heap_block_allocator::malloc(const std::size_t bytes)
{
	return static_cast<char*>(::HeapAlloc(instance()->heap_,0,bytes));
}

void private_heap_block_allocator::free(char * const block)
{
	::HeapFree(instance()->heap_,0,block);
}

} // namespace detail

} // namespace io
