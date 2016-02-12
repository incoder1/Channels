#include "heapallocator.hpp"

namespace io {

namespace sys {

namespace detail {

spin_lock heap_allocator::_mtx;
boost::atomic<heap_allocator*> heap_allocator::_instance(NULL);

} // namespace detail

} // namespace sys

}  // namespace io
