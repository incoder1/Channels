#include "prchdrs.h"
#include "smallobject.hpp"
#include "system.hpp"

#include <boost/atomic/atomic.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/pool/pool.hpp>
#include <boost/thread/thread.hpp>

namespace io {

typedef boost::default_user_allocator_malloc_free block_allocator;

namespace detail {

// Spinlock
class Spinlock:private boost::noncopyable {
private:
  static const uint8_t MAX_SPIN;
  typedef enum {LOCKED, UNLOCKED} LockState;
public:
  explicit Spinlock() BOOST_NOEXCEPT_OR_NOTHROW:
	state_(UNLOCKED)
  {}
  inline void lock() BOOST_NOEXCEPT_OR_NOTHROW
  {
  	uint8_t spinCount = 0;
    while (state_.exchange(LOCKED, boost::memory_order_acquire) == LOCKED) {
      if(++spinCount == MAX_SPIN) {
		  spinCount = 0;
		  boost::this_thread::yield();
      }
    }
  }
  BOOST_FORCEINLINE void unlock() BOOST_NOEXCEPT_OR_NOTHROW
  {
    state_.store(UNLOCKED, boost::memory_order_release);
  }
private:
	boost::atomic<LockState> state_;
};

const uint8_t Spinlock::MAX_SPIN = 8;

// synchronized pool
class SynchPool:public boost::noncopyable {
public:
	explicit SynchPool(const std::size_t size):
		spinlock_(),
		pool_(size)
	{}
	BOOST_FORCEINLINE void* malloc BOOST_PREVENT_MACRO_SUBSTITUTION() {
		spinlock_.lock();
		void *result = pool_.malloc();
		spinlock_.unlock();
		return result;
	}
	BOOST_FORCEINLINE void free BOOST_PREVENT_MACRO_SUBSTITUTION(void * const ptr) {
		spinlock_.lock();
		pool_.free(ptr);
		spinlock_.unlock();
	}
private:
	Spinlock spinlock_;
	boost::pool<block_allocator> pool_;
};

// Small Object ObjectAllocator
class ObjectAllocator:private boost::noncopyable {
public:

	static BOOST_FORCEINLINE void* allocate(std::size_t size) throw (std::bad_alloc) {
		return instance()->malloc(size);
	}

	static BOOST_FORCEINLINE void release(void *ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
		instance()->free(ptr,size);
	}

	~ObjectAllocator() BOOST_NOEXCEPT_OR_NOTHROW {
		for(std::size_t i = 0; i < MAX_SIZE; i++) {
			delete pools_[i];
		}
	}

private:
	typedef SynchPool pool_t;
	static const std::size_t MAX_SIZE = 128;

	ObjectAllocator() BOOST_NOEXCEPT_OR_NOTHROW {
		for(std::size_t i = 0; i < MAX_SIZE; i++) {
			pools_[i] = new pool_t(i+1);
		}
	}

	static void release() BOOST_NOEXCEPT_OR_NOTHROW {
		delete _instance.load(boost::memory_order_consume);
		_instance.store(NULL, boost::memory_order_release);
	}

	static ObjectAllocator* instance() {
		ObjectAllocator * tmp = _instance.load(boost::memory_order_consume);
		if (!tmp) {
			boost::mutex::scoped_lock guard(_mutex);
			tmp = _instance.load(boost::memory_order_consume);
			if (!tmp) {
				tmp = new ObjectAllocator();
				_instance.store(tmp, boost::memory_order_release);
				std::atexit(&ObjectAllocator::release);
			}
		}
		return tmp;
	}

	BOOST_FORCEINLINE pool_t* pool(std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
		return pools_[size - 1];
	}

	void* malloc BOOST_PREVENT_MACRO_SUBSTITUTION(std::size_t size) {
		void * result = NULL;
		if( (size-1) < MAX_SIZE) {
			result = pool(size)->malloc();
			if(NULL == result) {
				boost::throw_exception(std::bad_alloc());
			}
		} else {
			result = static_cast<void*>(block_allocator::malloc(size));
		}
		return result;
	}

	void free BOOST_PREVENT_MACRO_SUBSTITUTION(void *ptr, std::size_t size) {
		if(size-1 >= MAX_SIZE) {
			block_allocator::free(static_cast<char*>(ptr));
		} else {
			pool(size)->free(ptr);
		}
	}
private:
	static boost::mutex _mutex;
	static boost::atomic<ObjectAllocator*> _instance;
	pool_t* pools_[MAX_SIZE];
};

boost::mutex ObjectAllocator::_mutex;
boost::atomic<ObjectAllocator*> ObjectAllocator::_instance(NULL);

} // namespace detail

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
	return detail::ObjectAllocator::allocate(size);
}

void object::operator delete(void *ptr,std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
	detail::ObjectAllocator::release(ptr,size);
}


} // namespace io
