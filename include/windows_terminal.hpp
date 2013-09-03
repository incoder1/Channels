#ifndef WINDOWS_TERMINAL_HPP_INCLUDED
#define WINDOWS_TERMINAL_HPP_INCLUDED

#include <Windows.h>
#include "terminal.hpp"

namespace gui {

class CHANNEL_PUBLIC WindowsTerminal:public Terminal {
private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam) {
		//const WindowsTerminal *thisTerm = reinterpret_cast<WindowsTerminal*>(lParam);
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}
private:
	HWND hWnd_;
public:
	WindowsTerminal(uint16_t width, uint16_t heigth) BOOST_NOEXCEPT;
	void show();
};

} // namespace gui

#endif // WINDOWS_TERMINAL_HPP_INCLUDED
