#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

//#include <windows.h>
//#include <terminal.hpp>
//#include <windows_terminal.hpp>
//
//#include <file.hpp>
//#include <cstdio>
//#include <cstring>

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


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR _lpCmdLine, int nCmdShow)
int main(int atgc, const WCHAR** argv)
{
//	gui::WindowsTerminal term(80,25);
//	term.show();
//
//	term.put(msg);
//	RunLoop loop;
//	loop.start();

	io::PWriteChannel out = io::Console::outChanell();
	std::wstring msg = L"ASCII     abcde xyz \
					   German    äöü ÄÖÜ ß \
					   Polish    ąęźżńł \
					   Russian   абвгдеж эюя CJK       你好";

	::SetConsoleOutputCP(CP_WINUNICODE);

	DWORD output;
	::WriteConsoleW(::GetStdHandle(STD_OUTPUT_HANDLE),(LPVOID)msg.data(),msg.length(),&output,NULL);

//	io::byte_buffer buff = io::new_byte_byffer(msg.length()*sizeof(wchar_t));
//	buff.put(reinterpret_cast<const uint8_t*>(msg.data()),msg.length()*sizeof(wchar_t));
//	buff.flip();
//	out->write(buff);

	return EXIT_SUCCESS;
}
