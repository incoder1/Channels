#ifndef WINDOWS_TERMINAL_HPP_INCLUDED
#define WINDOWS_TERMINAL_HPP_INCLUDED

#include <string>

#include <Windows.h>
#include <Winuser.h>
#include <GdiPlus.h>
#include <boost/shared_ptr.hpp>

#include "terminal.hpp"
#include "windows_console_control.hpp"

namespace gui {

class GDIPlusContext {
private:
	ULONG_PTR gdiplusToken_;
public:
	GDIPlusContext();
	~GDIPlusContext();
	inline boost::shared_ptr<Gdiplus::Graphics> getGraphics(HDC dc) const {
		return boost::shared_ptr<Gdiplus::Graphics>(new Gdiplus::Graphics(dc));
	}
};

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
		wcex.hIcon          = LoadIconW(hInstance, IDI_APPLICATION);
		wcex.hCursor        = LoadCursorW(NULL, IDC_IBEAM);
		wcex.hbrBackground = ::CreateSolidBrush(RGB(0,0,0));
		wcex.lpszMenuName   = NULL;
		wcex.lpszClassName  = TERM_WINDOW_CLASS_NAME;
		wcex.hIconSm        = LoadIconW(wcex.hInstance, IDI_APPLICATION);
		wcex.hIconSm = NULL;
		return RegisterClassExW(&wcex);
	}
	LRESULT CALLBACK MessageRoute(HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam);
	void centerWindow();
	void onResize() const;
	void onPaint();
private:
	HWND hwnd_;
	GDIPlusContext gContext_;
	typedef std::wstring ScreenBuff;
	ScreenBuff scrBuff_;
public:
	WindowsTerminal(uint16_t width, uint16_t heigth) BOOST_NOEXCEPT;
	void put(const std::wstring& data);
	void show();
};


} // namespace gui

#endif // WINDOWS_TERMINAL_HPP_INCLUDED
