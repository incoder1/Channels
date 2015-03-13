#include "event.hpp"

namespace gui {


event::event(const event_type& type) BOOST_NOEXCEPT_OR_NOTHROW:
	type_(type)
{}

event_type event::type() const {
	return type_;
}

} // namesapce gui
