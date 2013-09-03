#ifndef _CHARSET_HPP_INCLUDED_
#define _CHARSET_HPP_INCLUDED_

#include <stdexcept>

namespace io {

template<typename _CharsetT>
class charset {
private:
	_CharsetT value_;
public:
	explicit charset(_CharsetT name) BOOST_NOEXCEPT:
		value_(name)
	{}
	inline _CharsetT id() const {
		return value_;
	}
};


class charset_exception:virtual std::runtime_error {
public:
	charset_exception(const char* message):
		std::runtime_error(message)
	{}
};

} // namespace io

#endif // _CHARSET_HPP_INCLUDED_
