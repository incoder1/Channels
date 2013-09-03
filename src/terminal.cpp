#include "terminal.hpp"

namespace gui {

Terminal::Terminal(uint16_t width, uint16_t heigth, size_t charSize) BOOST_NOEXCEPT:
	width_(width),
	heigth_(heigth),
	charSize_(charSize),
	screenBuff_(width*heigth*charSize)
{
}

void Terminal::updateBuffSize()
{
	screenBuff_.resize(width_*heigth_*charSize_);
}

Terminal::ScreenBuff Terminal::getBuff()
{
	return screenBuff_;
}

void Terminal::put(const io::byte_buffer& buff)
{
	if(screenBuff_.size()+buff.length() > screenBuff_.capacity() ) {
		screenBuff_.clear();
	}
	std::copy(buff.begin(), buff.last(), screenBuff_.end()-1);
	//screenBuff_//(buff.begin(), buff.last());
}

Terminal::~Terminal()
{
}

} // namespace gui
