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

#include <boost/unordered_set.hpp>

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


class XMLSyntax:private boost::noncopyable {
private:
	static const wchar_t WHITESPACES[26];
	static const wchar_t SYNTAX[6];
	boost::unordered_set<wchar_t> whitespaces_;
	boost::unordered_set<wchar_t> syntax_;
public:

	XMLSyntax() {
		uint8_t i;
		for(i=0; i < 26; i++) {
			whitespaces_.insert(WHITESPACES[i]);
		}
		for(i=0; i < 6; i++) {
			syntax_.insert(SYNTAX[i]);
		}
	}

	inline bool isWhiteSpace(const wchar_t charCode) {
		return whitespaces_.end() != whitespaces_.find(charCode);
	}
	inline bool __fastcall isSyntaxSymbol(const wchar_t charCode) {
		return syntax_.end() != syntax_.find(charCode);
	}
};


const wchar_t XMLSyntax::WHITESPACES[26] = {
		L'\u0009',L'\u000A',L'\u000B',L'\u000C',L'\u000D',
		L'\u0020',L'\u0085',L'\u00A0',L'\u2000',L'\u2001',
		L'\u2002',L'\u1680',L'\u180E',L'\u2003',L'\u2004',
		L'\u2005',L'\u2006',L'\u2007',L'\u2008',L'\u2009',
		L'\u200A',L'\u2028',L'\u2029',L'\u202F',L'\u205F',
		L'\u3000' };

const wchar_t XMLSyntax::SYNTAX[6] = {
		L'<',L'>',L'?',L'=',L'\'',L'&'
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
