#ifndef TERMINAL_HPP_INCLUDED
#define TERMINAL_HPP_INCLUDED

#include <boost/shared_array.hpp>

#include "channels_config.h"
#include "bytebuffer.hpp"

namespace gui {

struct Position {
	uint16_t x, y;
};

class Terminal {
public:
	typedef boost::shared_array<uint8_t> ScreenBuff;
private:
	uint16_t width_;
	uint16_t heigth_;
	size_t charSize_;
	Position pos_;
public:
	Terminal(uint16_t width, uint16_t height, size_t charSize) BOOST_NOEXCEPT;
	inline size_t getCharSize() const {
		return charSize_;
	}
	inline uint16_t getWidth() const {
		return width_;
	}
	inline void setWidth(uint16_t width) {
		width_ = width;
	}
	inline uint16_t getHeigth() const {
		return heigth_;
	}
	inline void setHeigth(uint16_t heigth) {
		heigth_ = heigth;
	}
	inline void setCursorPosition(uint16_t x, uint16_t y)
	{
		pos_.x = x <  width_? x: width_;
		pos_.y = y < heigth_ ? y: heigth_;
	}
	Position getCursorPosition() const {
		return pos_;
	}
	virtual ~Terminal() = 0;
};

} // namespace gui

#endif // TERMINAL_HPP_INCLUDED
