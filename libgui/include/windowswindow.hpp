#ifndef __GUI_WINDOWS_WINDOW_HPP_INCLUDED__
#define __GUI_WINDOWS_WINDOW_HPP_INCLUDED__

#include <Windows.h>

#include "window.hpp"

namespace gui {

class WindowsWindow:public Window {
private:
	static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
private:
	HWND hWindow_;
};

} // namesapce gui

#endif // __GUI_WINDOWS_WINDOW_HPP_INCLUDED__
