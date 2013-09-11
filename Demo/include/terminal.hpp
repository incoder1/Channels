#ifndef TERMINAL_HPP_INCLUDED
#define TERMINAL_HPP_INCLUDED

#include <list>

#include "channels_config.h"
#include "bytebuffer.hpp"

namespace gui {

class Terminal {
private:
	uint16_t width_;
	uint16_t heigth_;
	size_t charSize_;
protected:
	inline size_t maxSize() {
		return width_*heigth_*charSize_;
	}
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
	virtual ~Terminal() = 0;
};

} // namespace gui

#endif // TERMINAL_HPP_INCLUDED
