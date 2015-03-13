#ifndef __GUI_EVENT_HPP_INCLUDED__
#define __GUI_EVENT_HPP_INCLUDED__

#include <smallobject.hpp>
#include <object.hpp>

namespace gui {

typedef uint32_t event_type;

class event
{
protected:
	event(const event_type& type) BOOST_NOEXCEPT_OR_NOTHROW;
public:
	event_type type() const;
private:
	event_type type_;
};


} // namespace gui

#endif // __GUI_EVENT_HPP_INCLUDED__
