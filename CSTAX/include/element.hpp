#ifndef ELEMENT_HPP_INCLUDED
#define ELEMENT_HPP_INCLUDED

#include <boost/move/move.hpp>

namespace xml {

class Attribute {
private:
	const char* name_;
	const char* value_;
public:
	Attribute(const char* name, const char* value) BOOST_NOEXCEPT_OR_NOTHROW:
		name_(name),value_(value)
	{}
	inline const char* getName() const {
		return name_;
	}
	inline const char* getValue() const {
		return value_;
	}
};

} // namesapce xml

#endif // ELEMENT_HPP_INCLUDED
