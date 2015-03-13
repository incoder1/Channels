#include "abstractvisual.hpp"

namespace gui {

// AbstractContainter
AbstractVisual::AbstractVisual(AbstractVisual* const parent,const bounds& bounds) BOOST_NOEXCEPT_OR_NOTHROW:
	Widget(parent),
	bounds_(bounds)
{}

bool AbstractVisual::move(const point& location)
{
	bounds_.left = location.x;
	bounds_.top = location.y;
	return true;
}

bool AbstractVisual::resize(std::size_t width,std::size_t height)
{
	bounds_.width = width;
	bounds_.height = height;
	return true;
}

inline bounds AbstractVisual::getBounds() const {
	return bounds_;
}


} // namespace gui
