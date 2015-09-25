#ifndef __IO_HELPERS_HPP_INCLUDED__
#define __IO_HELPERS_HPP_INCLUDED__

#include "errors.hpp"

namespace io {

inline void* vpos(const byte_buffer& buff) {
	return reinterpret_cast<void*>(buff.begin().ptr());
}

inline void validate_io(ssize_t res, const char* msg) {
	if(res < 0 ) {
        boost::system::error_code ec(errno, boost::system::system_category());
		std::string smsg(msg);
		smsg.append(" ");
		smsg.append(ec.message());
		boost::throw_exception(io_exception(smsg));
	}
}

} // namesepace io

#endif // __IO_HELPERS_HPP_INCLUDED__
