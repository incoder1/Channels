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


#ifdef PLATFROM_WINDOWS
const char* LOCALE_CH = "CP1251";
const char* UTF16 = "UTF-16LE";
#else
const char* LOCALE_CH = "UFT-8";
const char* UTF16 = "UTF-16LE";
#endif

void change_console_charset() {
	io::CharsetFactory chf;
	io::Console::setCharset(chf.forName(LOCALE_CH));
}

#ifndef _MSC_VER
int main(int argc, const char** argv)
#else
int _tmain(int argc, TCHAR *argv[])
#endif
{
	change_console_charset();
	typedef io::Writer<std::wstring> uwriter;
	typedef io::Reader<std::wstring> ureader;
	try {
		uwriter out(io::Console::outChanell(), io::iconv_conv(UTF16,LOCALE_CH));
		out.write(L"Hello World. Привет мир\n\r");
		io::byte_buffer readBuff = io::new_byte_byffer(512);
		ureader in(io::Console::inChanell(),readBuff,io::iconv_conv(LOCALE_CH,UTF16));
		out.write(in.read());
	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}
