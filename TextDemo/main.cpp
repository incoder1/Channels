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

#if __cplusplus >= 201103L
#	define HAS_CPP11
#endif // __cplusplus

// Only to handle exception if any
#include <iostream>
#include <console.hpp>
#include <readwrite.hpp>
#include <datachannel.hpp>
#include <pipe.hpp>

//#include <network.hpp>

#if	defined(PLATFROM_WINDOWS)
	const char* LOCALE_CH = "CP1251";
	inline io::SConverter to_console_conv() {
		return io::new_converter("UTF-8",LOCALE_CH);
	}
	inline io::SConverter from_console_conv() {
		return io::new_converter(LOCALE_CH,"UTF-8");
	}
#elif defined(PLATFROM_UNIX)
	inline io::SConverter to_console_conv() {
		return io::char_empty_converter();
	}
	inline io::SConverter from_console_conv() {
		return io::char_empty_converter();
	}
#else
#	error "This operating system is not supported yet"
#endif

#ifdef HAS_CPP11
#	define U8(str) u8##str
#else
#	define U8(str) str
#endif // HAS_CPP11

typedef std::string ustring;


void change_console_charset()
{
	io::CharsetFactory chf;
	io::Console::setCharset(chf.forName(LOCALE_CH));
}

void pipe_write_routine(io::SPipe pipe) {
	typedef io::Writer<std::string> awriter;
	try {
		awriter out(pipe->sink(),io::char_empty_converter());
		out.write(U8("Hello from Pipe"));
	} catch(std::exception& exc) {
		std::cerr<<exc.what()<<std::endl;
	}
}

#ifndef _MSC_VER
int main(int argc, const char** argv)
#else
int _tmain(int argc, TCHAR *argv[])
#endif
{
	change_console_charset();
	typedef io::Writer<ustring> uwriter;
	typedef io::Reader<ustring> ureader;
	try {
		// char set conversation sample
		uwriter out(io::Console::outChanell(), to_console_conv());
		out.write(U8("Hello world English version. Привет мир, русская верссия\n\r"));
		io::byte_buffer readBuff = io::new_byte_byffer(512);
		ureader in(io::Console::inChanell(),readBuff, from_console_conv());
		out.write(U8("Type something :> "));
		out.write(in.read());

		// data channel sample
		char msg[14];
		io::DataChannel dch((void*)msg, std::strlen(msg));
		typedef io::Writer<std::string> awriter;
		awriter aout(io::Console::outChanell(), io::char_empty_converter());
		aout.write("Hello from data channel\n");

		// pipe sample
		io::SPipe pipe = io::open_pipe();
		boost::thread writeThread(boost::bind(pipe_write_routine,pipe));
		writeThread.start_thread();
		io::Reader<std::string> reader(pipe->source(), readBuff, io::char_empty_converter());
		std::string str = reader.read();
		aout.write(str);
	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}

