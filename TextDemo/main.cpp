// In case of Microsoft VC recode this file using UTF-16LE
// to enable Unicode string literals in cl
#ifndef UNICODE
#	define UNICODE
#endif
#ifndef _UNICODE
#	define _UNICODE
#endif

#if _MSC_VER
#	include <tchar.h>
#endif

#ifdef _WIN32
#	include "winver.h"
#endif // _WIN32

// Only to handle exception if any
#include <iostream>
#include <console.hpp>
#include <readwrite.hpp>
#include <datachannel.hpp>

//#include <network.hpp>

#ifdef _WIN32
const char* LOCALE_CH = "CP1251";
const char* UTF16 = "UTF-16LE";
#else
const char* LOCALE_CH = "UFT-8";
const char* UTF16 = "UTF-16LE";
#endif

void change_console_charset()
{
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
		uwriter out(io::Console::outChanell(), io::new_converter(UTF16,LOCALE_CH));
		out.write(L"Hello world English version. Привет мир, русская верссия\n\r");
		io::byte_buffer readBuff = io::new_byte_byffer(512);
		ureader in(io::Console::inChanell(),readBuff,io::new_converter(LOCALE_CH,UTF16));
		out.write(L"Type something :> ");
		out.write(in.read());

		char msg[14];
		io::DataChannel dch((void*)msg, std::strlen(msg));


		typedef io::Writer<std::string> awriter;
		awriter aout(io::Console::outChanell(), io::char_empty_converter());
		aout.write("Hello world\n");
	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}
