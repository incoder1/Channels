#include "prchdrs.h"
#include "smallobject.hpp"

#include <boost/make_shared.hpp>
#include <boost/pool/pool.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/once.hpp>
#include <boost/thread/mutex.hpp>

namespace io {

// Small Object object_allocator

class object_allocator {
public:
	static inline void* allocate(std::size_t size) throw (std::bad_alloc) {
		return instance()->malloc(size);
	}
	static inline void release(void *ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
		instance()->free(ptr,size);
	}
private:
	typedef boost::default_user_allocator_new_delete system_object_allocator;
	typedef typename boost::pool<system_object_allocator> pool_t;
	static const std::size_t MAX_SIZE = 128;
	object_allocator() BOOST_NOEXCEPT_OR_NOTHROW
	{}
	static object_allocator* const instance() throw(std::bad_alloc) {
		boost::call_once(&object_allocator::initilize,_once);
		return _instance;
	}
	static void initilize() throw(std::bad_alloc) {
		_instance = new object_allocator();
		std::atexit(&object_allocator::utilize);
	}
	static void utilize() BOOST_NOEXCEPT_OR_NOTHROW {
		delete _instance;
	}
	inline pool_t* get(const std::size_t size) {
		return pools_[size - 1].get();
	}
	void* malloc(std::size_t size) throw(std::bad_alloc) {
		void * result = NULL;
		if( (size-1) < MAX_SIZE) {
			pool_t* pool = get(size);
			if(NULL == pool) {
				boost::unique_lock<boost::mutex> lock(mutex_);
				pool = get(size);
				if(NULL == pool) {
					pools_[size - 1].reset(pool = new pool_t(size));
				}
			}
			result = pool->malloc();
			if(NULL == result) {
				boost::throw_exception(std::bad_alloc());
			}
		} else {
			result = static_cast<void*>(system_object_allocator::malloc(size));
		}
		return result;
	}
	void free(void *ptr, std::size_t size) {
		if(size-1 >= MAX_SIZE) {
			system_object_allocator::free(static_cast<char*>(ptr));
		} else {
			get(size)->free(ptr);
		}
	}
private:
	static object_allocator* _instance;
	static boost::once_flag _once;
	boost::mutex mutex_;
	boost::shared_ptr<pool_t> pools_[MAX_SIZE];
};

object_allocator* object_allocator::_instance = NULL;
boost::once_flag object_allocator::_once = BOOST_ONCE_INIT;

// object
object::object() BOOST_NOEXCEPT_OR_NOTHROW
{}


object::~object() BOOST_NOEXCEPT_OR_NOTHROW
{}

std::size_t object::hash() const BOOST_NOEXCEPT_OR_NOTHROW
{
	return reinterpret_cast<std::size_t>(this);
}

bool object::equal(object* const obj) const
{
	return this == obj;
}

void* object::operator new(std::size_t size) throw(std::bad_alloc)
{
	return object_allocator::allocate(size);
}

void object::operator delete(void *ptr,std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
	object_allocator::release(ptr,size);
}

} // namespace io
