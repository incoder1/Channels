#ifndef __GUI_ABSTRACT_VISUAL_HPP_INCLUDED__
#define __GUI_ABSTRACT_VISUAL_HPP_INCLUDED__

#include "widget.hpp"
#include "geometry.hpp"

namespace gui {

class AbstractVisual:public Widget {
public:
	AbstractVisual(AbstractVisual* const parent,const bounds& bounds) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual bool move(const point& location);
	virtual bool resize(std::size_t widht,std::size_t heigth);
	inline bounds getBounds() const;
	virtual void setVisible(bool visibility) = 0;
	virtual bool isVisible() const = 0;
private:
	bounds bounds_;
};

} // namesapce gui

#endif // __GUI_ABSTRACT_VISUAL_HPP_INCLUDED__
