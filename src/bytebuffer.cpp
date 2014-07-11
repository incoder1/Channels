#include "prchdrs.h"
#include "bytebuffer.hpp"

namespace io {

	byte_buffer::byte_buffer(boost::shared_array<uint8_t> data, uint8_t* const endp) BOOST_NOEXCEPT_OR_NOTHROW:
		basic_buffer<uint8_t>(data,endp)
	{}

	byte_buffer byte_buffer::new_heap_buffer(const std::size_t capacity) throw(std::bad_alloc) {
		byte_buffer_allocator::alloc_functor_t alloc(new_alloc<uint8_t>);
		byte_buffer_allocator::free_functor_t free(delete_free<uint8_t>);
		byte_buffer_allocator all(alloc,free);
		return all.allocate(capacity);
	}

} //namaspace io
