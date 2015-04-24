#include "prchdrs.h"
#include "memory.hpp"

namespace io {

	namespace detail {

		class lock_t {
		BOOST_MOVABLE_BUT_NOT_COPYABLE(lock_t)
		public:
			lock_t(boost::atomics::atomic_size_t* count):
				count_(count)
			{
				std::size_t lc;
				do {
					lc = count_->load(boost::memory_order::memory_order_consume);
				} while (!count_->compare_exchange_strong(lc, lc + 1, boost::memory_order::memory_order_release, boost::memory_order::memory_order_relaxed));
			}
			~lock_t() BOOST_NOEXCEPT_OR_NOTHROW {
				count_->fetch_sub(1);
			}
		private:
			boost::atomics::atomic_size_t* count_;
		};

		// private_heap_block_allocator
		private_heap_block_allocator* private_heap_block_allocator::_instance = NULL;
		boost::once_flag private_heap_block_allocator::_once = BOOST_ONCE_INIT;

		private_heap_block_allocator::private_heap_block_allocator():
			heap_(NULL),
			countdown_(0)
		{
			heap_ = ::HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_NO_SERIALIZE, 0, 0);
		}

		private_heap_block_allocator::~private_heap_block_allocator() BOOST_NOEXCEPT_OR_NOTHROW
		{
			::HeapDestroy(heap_);
		}

		char* private_heap_block_allocator::allocate(const std::size_t size) {
			lock_t lk(&countdown_);
			return static_cast<char*>(::HeapAlloc(heap_,0,size));
		}

		void private_heap_block_allocator::dispose(char* const block) {
			lock_t lk(&countdown_);
			::HeapFree(heap_,0,block);
		}

		void private_heap_block_allocator::initialize()
		{
			_instance = new private_heap_block_allocator();
			std::atexit(&private_heap_block_allocator::release);
		}

		void private_heap_block_allocator::release()
		{
			delete _instance;
		}

	} // namespace detail

} // namespace io
