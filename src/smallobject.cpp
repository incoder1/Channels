#include "prchdrs.h"
#include "smallobject.hpp"

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/pool/pool.hpp>
#include <boost/thread/once.hpp>
#include <boost/thread/mutex.hpp>

namespace io {

// Small Object allocator

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
	typedef boost::default_user_allocator_new_delete system_allocator;
	typedef typename boost::pool<system_allocator> pool_t;
	static const std::size_t MAX_SIZE = 128;
	SmallObjectAllocator():
		pools_(MAX_SIZE)
	{}
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
		void * result = NULL;
		if(size-1 < MAX_SIZE) {
			boost::shared_ptr<pool_t> pool = pools_[size-1];
			if(0 == pool.get()) {
				boost::unique_lock<boost::mutex> lock(mutex_);
				pool = pools_[size-1];
				if(NULL == pool) {
					pool = boost::make_shared<pool_t>(size-1);
					pools_[size-1] = pool;
				}
			}
			result = pool->malloc();
		} else {
			result = static_cast<void*>(system_allocator::malloc(size));
		}
		return result;
	}
	void free(void *ptr, std::size_t size) {
		if(size-1 >= MAX_SIZE) {
			system_allocator::free(static_cast<char*>(ptr));
		} else {
			pools_[size-1]->free(ptr);
		}
	}
private:
	static SmallObjectAllocator* _instance;
	static boost::once_flag _once;
	boost::mutex mutex_;
	std::vector<boost::shared_ptr<pool_t>> pools_;
};

SmallObjectAllocator* SmallObjectAllocator::_instance = NULL;
boost::once_flag SmallObjectAllocator::_once = BOOST_ONCE_INIT;

// object
object::object() BOOST_NOEXCEPT_OR_NOTHROW
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
