#ifndef ELEMENT_HPP_INCLUDED
#define ELEMENT_HPP_INCLUDED

#include <boost/move/move.hpp>

namespace xml {

class Attribute {
public:
	Attribute(const char* name, const char* value) BOOST_NOEXCEPT_OR_NOTHROW:
		name_(name),value_(value)
	{}
	inline std::string name() const {
		return name_;
	}
	inline std::string value() const {
		return value_;
	}
private:
	const std::string name_;
	const std::string value_;
};

} // namesapce xml

#endif // ELEMENT_HPP_INCLUDED
