#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <terminal.hpp>
#include <windows_terminal.hpp>

#include <Console.hpp>

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
	gui::WindowsTerminal term(80,100);
	term.show();

	std::wstring msg;
	const wchar_t * str = L"Привет! Hello! 嗨, やあ, Szia Привет! Hello! 嗨, やあ, Szia Привет! Hello! 嗨, やあ, Szia \n";
	for(int i=0; i<25; i++) {
		msg.append(str);
	}

	term.put(msg);
	RunLoop loop;
	loop.start();

//	io::PWriteChannel out = io::Console::outChanell();
//	io::byte_buffer buff = io::new_byte_byffer(msg.length()*sizeof(wchar_t));
//	buff.put((uint8_t*)msg.data(), msg.length()*sizeof(wchar_t));
//	buff.flip();
//	out->write(buff);

	return EXIT_SUCCESS;
}
