#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <terminal.hpp>
#include <windows_terminal.hpp>

#include <file.hpp>
#include <cstdio>
#include <cstring>

class RunLoop {
private:
	static DWORD WINAPI runLoopRoutine(void *attr) {
		// Main message loop:
		MSG msg;
		while(GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return 0;
	}
public:
	void start() throw(std::runtime_error) {
		runLoopRoutine(nullptr);
	}
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR _lpCmdLine, int nCmdShow)
{
	gui::WindowsTerminal term(80,25);
	term.show();
	std::wstring msg = L"ASCII     abcde xyz \
					   German    äöü ÄÖÜ ß \
					   Polish    ąęźżńł \
					   Russian   абвгдеж эюя CJK       你好";
	term.put(msg);
	RunLoop loop;
	loop.start();
	return EXIT_SUCCESS;
}
