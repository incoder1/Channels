#include "windows_terminal.hpp"

namespace gui {

const wchar_t* WindowsTerminal::TERM_WINDOW_CLASS_NAME = L"__CHANNEL_TERMINAL_WINDOW_CLASS__";

WindowsTerminal::WindowsTerminal(uint16_t width, uint16_t heigth) BOOST_NOEXCEPT:
		Terminal( width, heigth, sizeof(wchar_t) ),
		hwnd_(NULL)
{}

LRESULT CALLBACK WindowsTerminal::MessageRoute(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) const
{
	switch(msg) {
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_MOVE:
		this->onResize();
		return 0;
	default:
		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void WindowsTerminal::onResize() const {
	RECT rect;
	::GetWindowRect(hwnd_, &rect);
	console_.resize(rect.right, rect.bottom);
}

void WindowsTerminal::centerWindow()
{
	RECT rc;
	::GetWindowRect(hwnd_, &rc) ;
	::SetWindowPos(hwnd_, 0,
	             (GetSystemMetrics(SM_CXSCREEN) - rc.right)/2,
	             (GetSystemMetrics(SM_CYSCREEN) - rc.bottom)/2,
	             0, 0, SWP_NOZORDER|SWP_NOSIZE);
}

void WindowsTerminal::show()
{
	bool windowClassRegistered = registerWindowClass();
	if(!windowClassRegistered) {
		MessageBoxW(NULL,L"Call to RegisterClassEx failed!", L"Terminal", NULL);
		return;
	}

	hwnd_ = ::CreateWindowExW(
	            WS_EX_APPWINDOW,
	            TERM_WINDOW_CLASS_NAME,
	            L"Terminal",
	            WS_TILEDWINDOW,
	            0,0,
	            640,480,
	            NULL,
	            NULL,
	            GetModuleHandle(0),
	            NULL);

	::SetWindowLongPtr(hwnd_, GWL_USERDATA, (LONG_PTR)this);

	centerWindow();
	console_.inject(hwnd_);

	std::wcout<<L"Hello world!"<<std::endl;
	console_.redirectStdOut();
	std::wcout<<L"Привет Мир!"<<std::endl;

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	::ShowWindow(hwnd_,SW_SHOWDEFAULT);
	::UpdateWindow(hwnd_);
}


} // namespace gui
