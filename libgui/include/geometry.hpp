#ifndef __GUI_GEOMETRY_HPP_INCLUDED__
#define __GUI_GEOMETRY_HPP_INCLUDED__

namespace gui {

	struct point {
		std::size_t x;
		std::size_t y;
	};

	struct rect {
		point topLeft;
		point buttomRight;
	};

	inline rect create_rect(std::size_t left,std::size_t top, std::size_t right, std::size_t buttom) {
		return { {left, top}, {right, buttom} };
	}

	struct bounds {
		std::size_t left;
		std::size_t top;
		std::size_t width;
		std::size_t height;
	};

	inline bounds create_bounds(std::size_t left, std::size_t top, std::size_t width,std::size_t height) {
		bounds result = {left,top,width,height};
		return result;
	}

} // namesapce gui

#endif // __GUI_GEOMETRY_HPP_INCLUDED__
