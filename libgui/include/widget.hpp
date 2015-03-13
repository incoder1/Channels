#ifndef _LIBGUI_WIDGET_HPP_INCLUDED_
#define _LIBGUI_WIDGET_HPP_INCLUDED_

#include <boost/signals2/signal.hpp>
#include <boost/unordered_map.hpp>

#include "object.hpp"
#include "event.hpp"

namespace gui {

class Widget;

class WidgetEvent:public event {
public:
	WidgetEvent(const event_type& type,Widget* const source) BOOST_NOEXCEPT_OR_NOTHROW;
	const Widget* source() const {
		return source_;
	}
private:
	const Widget* source_;
};


class Widget:public object
{
	public:
		typedef boost::signals2::signal<void(const WidgetEvent*)> signall_t;
		typedef typename signall_t::slot_type slot_t;
		Widget(Widget* const parent) BOOST_NOEXCEPT_OR_NOTHROW;
		virtual ~Widget() BOOST_NOEXCEPT_OR_NOTHROW;
		inline const Widget* parent() const;
	protected:
		void connect(event_type eventType,const slot_t& slot);
		virtual bool handleEvent(const WidgetEvent* event);
	private:
		typedef boost::unordered_map<event_type,signall_t*> signall_table_t;
		signall_table_t signallTable_;
		Widget* const parent_;
};

} // namespace gui

#endif // _LIBGUI_WIDGET_HPP_INCLUDED_
