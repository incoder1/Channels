#include "prchdrs.h"
#include "smallobject.hpp"

#include <boost/atomic.hpp>
#include <boost/pool/pool.hpp>
#include <boost/thread/once.hpp>
#include <boost/thread/mutex.hpp>

namespace io {

// Small Object allocator
static const std::size_t MAX_SIZE = 128;

class SmallObjectAllocator {
public:
	static inline void* allocate(std::size_t size) throw (std::bad_alloc)
	{
		return instance()->malloc(size);
	}
	static inline void release(void *ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW
	{
		instance()->free(ptr,size);
	}
private:
	typedef boost::default_user_allocator_malloc_free system_allocator;
	typedef typename boost::pool<system_allocator> pool_t;

	SmallObjectAllocator() throw(std::bad_alloc) {
		for(std::size_t i=0; i <  MAX_SIZE; i++) {
			pools_[i].store(NULL,boost::memory_order_seq_cst);
		}
	}
	~SmallObjectAllocator() BOOST_NOEXCEPT_OR_NOTHROW {
		for(std::size_t i=0; i < MAX_SIZE; i++) {
			pool_t* pool = pools_[i].load(boost::memory_order_acquire);
			if(NULL != pool) {
				delete pool;
			}
			boost::atomic_thread_fence(boost::memory_order_release);
		}
	}
	static SmallObjectAllocator* const instance() throw(std::bad_alloc) {
		boost::call_once(&SmallObjectAllocator::initilize,_once);
		return _instance;
	}
	static void initilize() throw(std::bad_alloc) {
		_instance = new SmallObjectAllocator();
		std::atexit(&SmallObjectAllocator::utilize);
	}
	static void utilize() BOOST_NOEXCEPT_OR_NOTHROW {
		delete _instance;
	}
	void* malloc(std::size_t size) throw(std::bad_alloc) {
		if(size-1 < MAX_SIZE) {
			pool_t* pool = pools_[size-1].load(boost::memory_order_consume);
			if(NULL == pool) {
				boost::unique_lock<boost::mutex> lock(mutex_);
				pool = pools_[size-1].load(boost::memory_order_acquire);
				if(NULL == pool) {
					pool = new pool_t(size-1);
					pools_[size-1].exchange(pool, boost::memory_order_release);
				}
			} else {
				boost::atomic_thread_fence(boost::memory_order_release);
			}
			return pool->malloc();
		}
		return static_cast<void*>(system_allocator::malloc(size));
	}
	void free(void *ptr, std::size_t size) {
		if(size-1 >= MAX_SIZE) {
			system_allocator::free(static_cast<char*>(ptr));
		} else {
			pools_[size-1].load(boost::memory_order_relaxed)->free(ptr);
		}
	}
private:
	static SmallObjectAllocator* _instance;
	static boost::once_flag _once;
	boost::mutex mutex_;
	boost::atomic<pool_t*> pools_[MAX_SIZE];
};

SmallObjectAllocator* SmallObjectAllocator::_instance = NULL;
boost::once_flag SmallObjectAllocator::_once = BOOST_ONCE_INIT;

// object
object::object() BOOST_NOEXCEPT_OR_NOTHROW:
 boost::enable_shared_from_this<object>()
{}

object::~object() BOOST_NOEXCEPT_OR_NOTHROW
{}

std::size_t object::hash() const {
	return reinterpret_cast<std::size_t>(this);
}

bool object::equal(object* const obj) const {
	return this == obj;
}

void* object::operator new(std::size_t size) throw(std::bad_alloc)
{
	return SmallObjectAllocator::allocate(size);
}

void object::operator delete(void *ptr,std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW
{
	SmallObjectAllocator::release(ptr,size);
}

} // namespace io
