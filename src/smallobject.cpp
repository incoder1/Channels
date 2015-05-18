#include "prchdrs.h"
#include "smallobject.hpp"
#include "system.hpp"

#include <boost/atomic/atomic.hpp>
#include <boost/pool/pool.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

namespace io {

typedef boost::default_user_allocator_malloc_free block_allocator;

namespace detail {

class Spinlock:private boost::noncopyable {
private:
  static const uint8_t MAX_SPIN  = 8;
  typedef enum {Locked, Unlocked} LockState;
public:
  explicit Spinlock() : state_(Unlocked)
  {}
  void lock()
  {
  	uint8_t spinCount = 0;
    while (state_.exchange(Locked, boost::memory_order_acquire) == Locked) {
      if(++spinCount == MAX_SPIN) {
		  spinCount = 0;
		  boost::this_thread::yield();
      }
    }
  }
  void unlock()
  {
    state_.store(Unlocked, boost::memory_order_release);
  }
private:
	boost::atomic<LockState> state_;
};

// synchronized pool
class synch_pool:public boost::noncopyable {
private:
public:
	explicit synch_pool(const std::size_t size):
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

// Small Object object_allocator
class object_allocator:private boost::noncopyable {
public:

	static inline void* allocate(std::size_t size) throw (std::bad_alloc) {
		return const_cast<object_allocator*>(instance())->malloc(size);
	}

	static inline void release(void *ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
		const_cast<object_allocator*>(instance())->free(ptr,size);
	}

	~object_allocator() BOOST_NOEXCEPT_OR_NOTHROW {
		for(std::size_t i = 0; i < MAX_SIZE; i++) {
			delete pools_[i];
		}
	}

private:
	typedef synch_pool pool_t;
	static const std::size_t MAX_SIZE = 128;

	object_allocator() BOOST_NOEXCEPT_OR_NOTHROW {
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
				std::atexit(&object_allocator::release);
			}
		}
		return _instance;
	}

	void* malloc BOOST_PREVENT_MACRO_SUBSTITUTION(std::size_t size) {
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

	void free BOOST_PREVENT_MACRO_SUBSTITUTION(void *ptr, std::size_t size) {
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
volatile object_allocator* volatile object_allocator::_instance = NULL;

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
	return detail::object_allocator::allocate(size);
}

void object::operator delete(void *ptr,std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
	detail::object_allocator::release(ptr,size);
}

} // namespace io
