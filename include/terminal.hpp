#ifndef TERMINAL_HPP_INCLUDED
#define TERMINAL_HPP_INCLUDED

#include <vector>

#include "channels_config.h"
#include "bytebuffer.hpp"

namespace gui {

class CHANNEL_PUBLIC Terminal {
private:
	uint16_t width_;
	uint16_t heigth_;
	size_t charSize_;
	std::vector<uint8_t> screenBuff_;
	void updateBuffSize();
protected:
	typedef std::vector<uint8_t> ScreenBuff;
	inline ScreenBuff getBuff();
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
		updateBuffSize();
	}
	inline uint16_t getHeigth() const {
		return heigth_;
	}
	inline void setHeigth(uint16_t heigth) {
		heigth_ = heigth;
		updateBuffSize();
	}
	void put(const io::byte_buffer& buff);
	virtual ~Terminal();
};

} // namespace gui

#endif // TERMINAL_HPP_INCLUDED
