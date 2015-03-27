#ifndef ELEMENT_HPP_INCLUDED
#define ELEMENT_HPP_INCLUDED

#include <boost/move/move.hpp>

namespace xml {

class Attribute {
public:
	Attribute(const std::string& name, const std::string& value) BOOST_NOEXCEPT_OR_NOTHROW:
		name_(name),value_(value)
	{}
	inline const std::string name() const {
		return name_;
	}
	inline const std::string value() const {
		return value_;
	}
private:
	std::string name_;
	std::string value_;
};

} // namesapce xml

#endif // ELEMENT_HPP_INCLUDED
