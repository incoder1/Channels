#include "prchdrs.h"
#include "smallobjectpool.hpp"

namespace io {

namespace mmt {

static const std::size_t SMALL_OBJECT_LIMIT = 64;

/// !\brief Small object memory allocation
class SmallObjectPool:private boost::noncopyable {
private:
	static SmallObjectPool* _instance;
	static boost::once_flag _once;
	static void initilizeSigleton() {
		// we don't need any null check since once call
		_instance = new SmallObjectPool();
		std::atexit(&SmallObjectPool::utilizeSigleton);
	}
	static void utilizeSigleton(void) {
		// delete the singleton
		delete _instance;
	}
	typedef boost::pool<boost::default_user_allocator_malloc_free> pool_type;
	typedef boost::shared_ptr<pool_type> SPool;
	typedef boost::unordered_map< std::size_t, SPool > vpool_t;

	explicit SmallObjectPool() BOOST_NOEXCEPT_OR_NOTHROW
	{}

	inline SPool getPool(std::size_t size) {
		vpool_t::const_iterator it = vpool_.find(size);
		// double check that new pool is not created with other thread
		if(vpool_.end() == it) {
			// need write protection
			boost::unique_lock<boost::mutex> lock(mutex_);
			it = vpool_.find(size);
			if(vpool_.end() == it) {
				SPool result(new pool_type(size));
				vpool_.insert(std::make_pair(size,result));
				return result;
			}
		}
		return it->second;
	}

public:
	static SmallObjectPool* const instance() {
		// thread safety with call once
		boost::call_once(&SmallObjectPool::initilizeSigleton,_once);
		return _instance;
	}
	/// Allocates memory block for the small object, speed O(1)
	inline void* allocate(std::size_t size) throw(std::bad_alloc) {
		void* result = NULL;
		// apply segregate storage only for small objects
		if(size <= SMALL_OBJECT_LIMIT) {
			result = getPool(size)->ordered_malloc();
			if(NULL == result) {
				boost::throw_exception(std::bad_alloc());
			}
		} else {
			// not a small object use default allocator
			result = static_cast<void*>(new uint8_t [size]);
		}
		return result;
	}
	/// Free memory block
	inline void free(void *ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
		if(size > SMALL_OBJECT_LIMIT) {
			delete [] reinterpret_cast<uint8_t*>(ptr);
		} else {
			vpool_[size]->free(ptr);
		}
	}
private:
	boost::mutex mutex_;
	vpool_t vpool_;
};

// initialize the singleton global variables
SmallObjectPool* SmallObjectPool::_instance = NULL;

boost::once_flag SmallObjectPool::_once = BOOST_ONCE_INIT;

} // namesapace mmg

// Small Object
SmallObject::~SmallObject() BOOST_NOEXCEPT_OR_NOTHROW
{}

void * SmallObject::operator new(std::size_t size) throw(std::bad_alloc)
{
	return io::mmt::SmallObjectPool::instance()->allocate(size);
}

void SmallObject::operator delete(void* ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW
{
	io::mmt::SmallObjectPool::instance()->free(ptr,size);
}

}
