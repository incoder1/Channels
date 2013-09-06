#ifndef WINDOWS_TERMINAL_HPP_INCLUDED
#define WINDOWS_TERMINAL_HPP_INCLUDED

#include <Windows.h>
#include <gdiplus.h>
#include <boost/bind.hpp>

#include "terminal.hpp"
#include "windows_console_control.hpp"

namespace gui {

class WindowsTerminal:public Terminal {
private:
	static const wchar_t* TERM_WINDOW_CLASS_NAME;
	static LRESULT CALLBACK TerminalWndProc(HWND hwnd, UINT uMsg,WPARAM wParam, LPARAM lParam) {
		// Recover the pointer to our class, don't forget to type cast it back
		WindowsTerminal* pThis = (WindowsTerminal*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if(NULL == pThis) {
			return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
		} else {
			return pThis->MessageRoute(hwnd,uMsg,wParam,lParam);
		}
	}
	static bool registerWindowClass() {
		HINSTANCE hInstance = ::GetModuleHandle(0);
		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style          = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc    = TerminalWndProc;
		wcex.cbClsExtra     = 0;
		wcex.cbWndExtra     = 0;
		wcex.hInstance      = hInstance;
		wcex.hIcon          = LoadIconW(hInstance, MAKEINTRESOURCEW(32512));
		wcex.hCursor        = LoadCursorW(NULL, MAKEINTRESOURCEW(32512));
		wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName   = NULL;
		wcex.lpszClassName  = TERM_WINDOW_CLASS_NAME;
		wcex.hIconSm        = LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(32512));
		return RegisterClassExW(&wcex);
	}
	LRESULT CALLBACK MessageRoute(HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam) const;
	void centerWindow();
	void onResize() const;
private:
	HWND hwnd_;
	WindowsConsoleControl console_;
public:
	WindowsTerminal(uint16_t width, uint16_t heigth) BOOST_NOEXCEPT;
	void show();
};

} // namespace gui

#endif // WINDOWS_TERMINAL_HPP_INCLUDED
