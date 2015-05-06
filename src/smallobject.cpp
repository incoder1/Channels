#include "prchdrs.h"
#include "smallobject.hpp"

#include <boost/make_shared.hpp>
#include <boost/pool/pool.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/atomic.hpp>

#ifdef PLATFORM_WINDOWS
#	include "win/memory.hpp"
#endif // PLATFORM_WINDOWS

namespace io {

#ifdef PLATFORM_WINDOWS
#	include "win/memory.hpp"
typedef detail::private_heap_block_allocator block_allocator;
#else // UNIX
typedef boost::default_user_allocator_new_delete block_allocator;
#endif // PLATFROM_WINDOWS

// synchronized pool
class synch_pool:private boost::noncopyable
{
public:
	explicit synch_pool(const std::size_t size):
		pool_(size)
	{
	}
	inline void* malloc() {
		boost::unique_lock<boost::mutex> lock(mutex_);
		return pool_.malloc();
	}
	inline void free(void * const ptr) {
		boost::unique_lock<boost::mutex> lock(mutex_);
		pool_.free(ptr);
	}
private:
	boost::mutex mutex_;
	boost::pool<block_allocator> pool_;
};

// Small Object object_allocator
class object_allocator:private boost::noncopyable {
public:

	static void* allocate(std::size_t size) throw (std::bad_alloc) {
		return const_cast<object_allocator*>(instance())->malloc(size);
	}

	static void release(void *ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
		const_cast<object_allocator*>(instance())->free(ptr,size);
	}

	~object_allocator() BOOST_NOEXCEPT_OR_NOTHROW
	{
		for(std::size_t i = 0; i < MAX_SIZE; i++) {
			delete pools_[i];
		}
	}

private:
	typedef synch_pool pool_t;
	static const std::size_t MAX_SIZE = 128;
	object_allocator() BOOST_NOEXCEPT_OR_NOTHROW
	{
		for(std::size_t i = 0; i < MAX_SIZE; i++) {
			pools_[i] = new pool_t(i+1);
		}
	}

	static void release() BOOST_NOEXCEPT_OR_NOTHROW {
		delete _instance;
	}

	static volatile object_allocator* volatile instance() {
		if(NULL == _instance) {
			boost::unique_lock<boost::mutex> lock(_mutex);
			if(NULL == _instance) {
				_instance = new volatile object_allocator();
				#ifdef PLATFORM_WINDOWS
					block_allocator::instance();
				#endif
				std::atexit(&object_allocator::release);
			}
		}
		return _instance;
	}


	void* malloc(std::size_t size) {
		void * result = NULL;
		if( (size-1) < MAX_SIZE) {
			result = pools_[size - 1]->malloc();
			if(NULL == result) {
				boost::throw_exception(std::bad_alloc());
			}
		} else {
			result = static_cast<void*>(new uint8_t[size]);
		}
		return result;
	}

	void free(void *ptr, std::size_t size) {
		if(size-1 >= MAX_SIZE) {
			delete [] static_cast<uint8_t*>(ptr);
		} else {
			pools_[size - 1]->free(ptr);
		}
	}
private:
	static boost::mutex _mutex;
	static volatile object_allocator* volatile _instance;
	pool_t* pools_[MAX_SIZE];
};

boost::mutex object_allocator::_mutex;
volatile object_allocator* volatile object_allocator::_instance;

// object
object::object() BOOST_NOEXCEPT_OR_NOTHROW
{}


object::~object() BOOST_NOEXCEPT_OR_NOTHROW
{}

std::size_t object::hash() const BOOST_NOEXCEPT_OR_NOTHROW
{
	return reinterpret_cast<std::size_t>(this);
}

void* object::operator new(std::size_t size) throw(std::bad_alloc)
{
	return object_allocator::allocate(size);
}

void object::operator delete(void *ptr,std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
	object_allocator::release(ptr,size);
}

} // namespace io
