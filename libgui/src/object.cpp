#include "object.hpp"

namespace gui {

// object
object::object() BOOST_NOEXCEPT_OR_NOTHROW
{}

object::~object() BOOST_NOEXCEPT_OR_NOTHROW
{}

std::size_t object::hash() const {
	return reinterpret_cast<std::size_t>(this);
}

} // namespace gui
