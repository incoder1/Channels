#ifndef __IO_HELPERS_HPP_INCLUDED__
#define __IO_HELPERS_HPP_INCLUDED__

#include "errors.hpp"

inline void validate_io(ssize_t res, const char* msg) {
	if(-1 == res) {
		boost::system::error_code ec;
		std::string smsg(msg);
		smsg.append(" ");
		smsg.append(ec.message());
		boost::throw_exception(io_exception(smsg));
	}
}

#endif // __IO_HELPERS_HPP_INCLUDED__
