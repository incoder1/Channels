#include "widget.hpp"

namespace gui {

// WidgetEvent
WidgetEvent::WidgetEvent(const event_type& type,Widget* const source) BOOST_NOEXCEPT_OR_NOTHROW:
	event(type),
	source_(source)
{}

// Widget
Widget::Widget(Widget* const parent) BOOST_NOEXCEPT_OR_NOTHROW:
	object(),
	parent_(parent)
{}

void Widget::connect(event_type ev,const slot_t& slot) {
	signall_table_t::iterator it = signallTable_.find(ev);
	if( it != signallTable_.end() ) {
		it->second->connect(slot);
	} else {
		signall_t* sig = new signall_t();
		signallTable_.emplace(ev,sig);
		sig->connect(slot);
	}
}

inline const Widget* Widget::parent() const {
	return parent_;
}

Widget::~Widget() BOOST_NOEXCEPT_OR_NOTHROW
{
}

bool Widget::handleEvent(const WidgetEvent* event)
{
	signall_table_t::iterator it = signallTable_.find(event->type());
	bool result = it != signallTable_.end();
	if(result) {
		(*(it->second))(event);
	}
	return result;
}

} // namesapce Widget
