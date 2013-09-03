#include "windows_terminal.hpp"

namespace gui {

static const wchar_t TERM_WINDOW_CLASS_NAME[] = L"__CHANNEL_TERMINAL_WINDOW_CLASS__";

WindowsTerminal::WindowsTerminal(uint16_t width, uint16_t heigth) BOOST_NOEXCEPT:
Terminal( width, heigth, sizeof(wchar_t) )
{
}

void WindowsTerminal::show()
{
	HINSTANCE hInstance = GetModuleHandle(0);
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIconW(hInstance, MAKEINTRESOURCEW(32512));
	wcex.hCursor        = LoadCursorW(NULL, MAKEINTRESOURCEW(32512));
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = TERM_WINDOW_CLASS_NAME;
	wcex.hIconSm        = LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(32512));

	if(!RegisterClassExW(&wcex)) {
		MessageBoxW(NULL,L"Call to RegisterClassEx failed!", L"Win32 Guided Tour", NULL);
		return;
	}

	hWnd_ = CreateWindowW(
	            TERM_WINDOW_CLASS_NAME,
	            L"Terminal",
	            WS_OVERLAPPEDWINDOW,
	            CW_USEDEFAULT, CW_USEDEFAULT,
	            500, 100,
	            NULL,
	            NULL,
	            wcex.hInstance,
	            this
	        );

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	::ShowWindow(hWnd_,SW_SHOWDEFAULT);
	UpdateWindow(hWnd_);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

} // namespace gui
