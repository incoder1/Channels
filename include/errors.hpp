#ifndef __IO_ERRORS_HPP_INCLUDED__
#define __IO_ERRORS_HPP_INCLUDED__

#include <channels_config.h>

#include <stdexcept>
#include <string>

#include <boost/config.hpp>
#include <boost/system/error_code.hpp>

namespace io {

class io_exception: public std::runtime_error {
public:
	explicit io_exception(const std::string& what):
		std::runtime_error(what)
	{}
};

} // namespace io


#endif // __IO_ERRORS_HPP_INCLUDED__
