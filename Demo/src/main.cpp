#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <terminal.hpp>
#include <windows_terminal.hpp>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR _lpCmdLine, int nCmdShow)
{
	gui::WindowsTerminal terminal(80,25);
	terminal.show();
	return EXIT_SUCCESS;
}
