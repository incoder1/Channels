#include "windows_terminal.hpp"

//Trick for MinGW
#ifndef LWA_ALPHA
	#define LWA_ALPHA 0x00000002
	#define LWA_COLORKEY 0x00000001
	extern "C" {
		BOOL WINAPI SetLayeredWindowAttributes(HWND,COLORREF,BYTE,DWORD);
	}
#endif // LWA_ALPHA

namespace gui {

using namespace Gdiplus;


// GDI plus context
GDIPlusContext::GDIPlusContext():
	gdiplusToken_(0)
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken_, &gdiplusStartupInput, NULL);
}

GDIPlusContext::~GDIPlusContext()
{
	GdiplusShutdown(gdiplusToken_);
}

// WindowsTerminal
const wchar_t* WindowsTerminal::TERM_WINDOW_CLASS_NAME = L"__CHANNEL_TERMINAL_WINDOW_CLASS__";

WindowsTerminal::WindowsTerminal(uint16_t width, uint16_t heigth) BOOST_NOEXCEPT:
Terminal( width, heigth, sizeof(wchar_t) ),
          hwnd_(NULL),
          scrBuff_()
{}

LRESULT CALLBACK WindowsTerminal::MessageRoute(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		onResize();
		return 0;
	case WM_PAINT:
		onPaint();
		return 0;
	default:
		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void onKeyPressed(LPARAM lParam, WPARAM wParam) {
}

void WindowsTerminal::put(const std::wstring& str)
{
	if(scrBuff_.length() + str.length() >= maxSize()) {
		ScreenBuff::iterator last = scrBuff_.begin() +  (maxSize() / 2);
		scrBuff_.erase(scrBuff_.begin(), last);
	}
	scrBuff_.append(str);
	::UpdateWindow(hwnd_);
}

void WindowsTerminal::onResize() const
{
	::UpdateWindow(hwnd_);
}

void WindowsTerminal::onPaint()
{
	PAINTSTRUCT paintStruct;
	HDC dc = ::BeginPaint(hwnd_, const_cast<LPPAINTSTRUCT>(&paintStruct));

	boost::shared_ptr<Graphics> g = gContext_.getGraphics(dc);
	g->SetPageUnit(UnitPixel);

	StringFormat format;
    format.SetAlignment(StringAlignmentNear);
    format.SetLineAlignment(StringAlignmentNear);
    Font font(L"Consolas", 10, FontStyleBold);

    float fontHeigth = font.GetHeight(g.get());

    RECT cr;
    ::GetClientRect(hwnd_,&cr);
    RectF bounds(0, 0, float(cr.right), getHeigth()*fontHeigth);

    SolidBrush brush(Color::Green);

	g->DrawString(
				(const WCHAR*)scrBuff_.data(),
				(int)scrBuff_.length(),
				&font,
				bounds,
				&format,
				&brush);

	::EndPaint(hwnd_, const_cast<LPPAINTSTRUCT>(&paintStruct));
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
	            this);

	// Set WS_EX_LAYERED on this window
	::SetWindowLong(hwnd_, GWL_EXSTYLE,
        GetWindowLong(hwnd_, GWL_EXSTYLE) | WS_EX_LAYERED);

	::SetLayeredWindowAttributes(hwnd_, 0, (255 * 70) / 100, LWA_ALPHA);

	::SetWindowLongPtr(hwnd_, GWL_USERDATA, (LONG_PTR)this);

	::RedrawWindow(hwnd_, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME |
        RDW_ALLCHILDREN);

	centerWindow();

	::ShowWindow(hwnd_,SW_SHOWNORMAL);
	::UpdateWindow(hwnd_);
	::RedrawWindow(hwnd_,NULL,NULL,RDW_INVALIDATE);
}


} // namespace gui
