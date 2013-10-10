#ifndef UNICODE
#	define UNICODE
#endif
#ifndef _UNICODE
#	define _UNICODE
#endif

#if _MSC_VER
#	include <tchar.h>
#	include "winver.h"
#endif

// Only to handle exception if any
#include <iostream>
#include <console.hpp>
#include <readwrite.hpp>

#ifndef _MSC_VER
int main(int argc, const char** argv)
#else
int _tmain(int argc, TCHAR *argv[])
#endif
{
	typedef io::Writer<std::wstring> uwriter;
	typedef io::Reader<std::wstring> ureader;
	::SetConsoleCP(1251);
	::SetConsoleOutputCP(1251);
	try {
		uwriter out(io::Console::outChanell(), io::iconv_conv("UTF-16LE","CP1251"));
		out.write(L"Hello World. Привет мир\n\r");
		io::byte_buffer readBuff = io::new_byte_byffer(512);
		ureader in(io::Console::inChanell(),readBuff,io::iconv_conv("CP1251","UTF-16LE"));
		out.write(in.read());
	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}
