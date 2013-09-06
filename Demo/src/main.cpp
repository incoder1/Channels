#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <terminal.hpp>
#include <windows_terminal.hpp>

class RunLoop {
private:
	static DWORD WINAPI runLoopRoutine(void *attr) {
		// Main message loop:
		MSG msg;
		while(GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
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
	RunLoop loop;
	loop.start();
	return EXIT_SUCCESS;
}
