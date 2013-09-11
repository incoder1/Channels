#include "terminal.hpp"

namespace gui {

Terminal::Terminal(uint16_t width, uint16_t heigth, size_t charSize) BOOST_NOEXCEPT:
	width_(width),
	heigth_(heigth),
	charSize_(charSize)
{
}

Terminal::~Terminal()
{
}

} // namespace gui
