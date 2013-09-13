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
#include <xmlparser.hpp>

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

//	io::byte_buffer buff = io::new_byte_byffer(msg.length()*sizeof(wchar_t));
//	buff.put(reinterpret_cast<const uint8_t*>(msg.data()),msg.length()*sizeof(wchar_t));
//	buff.flip();
//	out->write(buff);

	io::File file("text.xml");
	if(!file.exist()) {
		file.create();
	}

	typedef xml::StreamWriter<char, xml::AlphabetUTF8> UTF8StreamWriter;
	typedef UTF8StreamWriter::Node Node;

	UTF8StreamWriter writer(file.openForWrite());
	Node node("configuration", "", "");
	writer.write(&node);

	return EXIT_SUCCESS;
}
