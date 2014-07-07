#include "prchdrs.h"
#include "smallobjectpool.hpp"

#ifdef PLATFORM_WINDOWS
#	ifndef HEAP_CREATE_ENABLE_EXECUTE
#		define HEAP_CREATE_ENABLE_EXECUTE 0x00040000
#	endif // HEAP_CREATE_ENABLE_EXECUTE
#endif // PLATFORM_WINDOWS

namespace io {

namespace mmt {

static const std::size_t SMALL_OBJECT_LIMIT = 64;

#ifdef PLATFORM_WINDOWS
class PrivateHeapAllocator {
public:
  typedef std::size_t size_type; //!< An unsigned integral type that can represent the size of the largest object to be allocated.
  typedef std::ptrdiff_t difference_type; //!< A signed integral type that can represent the difference of any two pointers.
private:
	static boost::once_flag _once;
	static PrivateHeapAllocator* _instance;
	static void initilizeSigleton() {
		// we don't need any null check since once call
		_instance = new PrivateHeapAllocator();
		std::atexit(&PrivateHeapAllocator::utilizeSigleton);
	}
	static void utilizeSigleton(void) {
		// delete the singleton
		delete _instance;
	}
	explicit PrivateHeapAllocator() throw(std::runtime_error):
		heap_(0)
	{
		heap_ = ::HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0 , 0);
		validate<std::runtime_error>(NULL != heap_, "Can not create private heap");
	}
	char inline *heapAlloc(const size_type bytes) const {
		void *res = ::HeapAlloc(heap_, 0, bytes);
		return static_cast<char *>(res);
	}
	void inline heapFree(char * const block) const {
		::HeapFree(heap_, 0, block);
	}
public:
	static const PrivateHeapAllocator* instance() throw(std::runtime_error) {
		// thread safety with call once
		boost::call_once(&PrivateHeapAllocator::initilizeSigleton,_once);
		return _instance;
	}
	static char * malloc BOOST_PREVENT_MACRO_SUBSTITUTION(const size_type bytes)
	{
		return instance()->heapAlloc(bytes);
	}
	static void free BOOST_PREVENT_MACRO_SUBSTITUTION(char * const block)
	{
		return instance()->heapFree(block);
	}
	~PrivateHeapAllocator() {
		::HeapDestroy(heap_);
	}
private:
	HANDLE heap_;
};

PrivateHeapAllocator* PrivateHeapAllocator::_instance = NULL;
boost::once_flag PrivateHeapAllocator::_once  = BOOST_ONCE_INIT;

typedef PrivateHeapAllocator heap_allocator;
#else
typedef boost::default_user_allocator_malloc_free heap_allocator;
#endif // PLATFORM_WINDOWS

/// !\brief Small object memory allocation
class SmallObjectAllocator:private boost::noncopyable {
private:
	static SmallObjectAllocator* _instance;
	static boost::once_flag _once;
	static void initilizeSigleton() {
#ifdef PLATFORM_WINDOWS
		// trick to delete private heap after small object allocator
		// and not the vise versa, which is default for the at exit
		PrivateHeapAllocator::instance();
#endif // PLATFORM_WINDOWS
		// we don't need any null check since once call
		_instance = new SmallObjectAllocator();
		std::atexit(&SmallObjectAllocator::utilizeSigleton);
	}
	static void utilizeSigleton(void) {
		// delete the singleton
		delete _instance;
	}
	typedef boost::pool<heap_allocator> pool_type;
	typedef boost::shared_ptr<pool_type> SPool;
	typedef boost::unordered_map<std::size_t,SPool> pool_table_t;

	explicit SmallObjectAllocator() BOOST_NOEXCEPT_OR_NOTHROW
	{}

	inline SPool getPool(std::size_t size) {
		pool_table_t::const_iterator it = pooltbl_.find(size);
		// double check that new pool is not created with other thread
		if(pooltbl_.end() == it) {
			// need write protection
			boost::unique_lock<boost::mutex> lock(mutex_);
			it = pooltbl_.find(size);
			if(pooltbl_.end() == it) {
				SPool result(new pool_type(size));
				pooltbl_.insert(std::make_pair(size,result));
				return result;
			}
		}
		return it->second;
	}
public:
	static SmallObjectAllocator* const instance() {
		// thread safety with call once
		boost::call_once(&SmallObjectAllocator::initilizeSigleton,_once);
		return _instance;
	}
	/// Allocates memory block for the small object, speed O(1)
	inline void* allocate(std::size_t size) throw(std::bad_alloc) {
		void* result = NULL;
		// apply segregate storage only for small objects
		if(size <= SMALL_OBJECT_LIMIT) {
			result = getPool(size)->malloc();
			if(NULL == result) {
				boost::throw_exception(std::bad_alloc());
			}
		} else {
			// not a small object use default allocator
			result = std::malloc(size);
		}
		return result;
	}
	/// Free memory block
	inline void free(void *ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW {
		if(size > SMALL_OBJECT_LIMIT) {
			std::free(ptr);
		} else {
			pooltbl_[size]->free(ptr);
		}
	}
private:
	boost::mutex mutex_;
	pool_table_t pooltbl_;
};

// initialize the singleton global variables
SmallObjectAllocator* SmallObjectAllocator::_instance = NULL;

boost::once_flag SmallObjectAllocator::_once = BOOST_ONCE_INIT;

} // namesapace mmg

// Small Object
SmallObject::~SmallObject() BOOST_NOEXCEPT_OR_NOTHROW
{}

void * SmallObject::operator new(std::size_t size) throw(std::bad_alloc)
{
	return io::mmt::SmallObjectAllocator::instance()->allocate(size);
}

void SmallObject::operator delete(void* ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW
{
	io::mmt::SmallObjectAllocator::instance()->free(ptr,size);
}

}
