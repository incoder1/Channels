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

const char* LOCALE_CH = "UTF-16LE";
typedef std::string ustring;

inline io::SConverter to_console_conv()
{
	return io::new_converter("UTF-8",LOCALE_CH);
}

inline io::SConverter from_console_conv()
{
	return io::new_converter(LOCALE_CH,"UTF-8");
}

#elif defined(PLATFROM_UNIX)

typedef std::string ustring;

inline io::SConverter to_console_conv()
{
	return io::char_empty_converter();
}
inline io::SConverter from_console_conv()
{
	return io::char_empty_converter();
}
#else
#	error "This operating system is not supported yet"
#endif


typedef io::Writer<ustring> uwriter;
typedef io::Reader<ustring> ureader;

void change_console_charset()
{
	io::Console::setCharset(io::Charsets::forName(LOCALE_CH));
}

void pipe_write_routine(io::SPipe pipe) throw(io::io_exception)
{
	try {
		uwriter out(pipe->sink(),io::char_empty_converter());
		out.write("Hello from Pipe");
	} catch(std::exception& exc) {
		std::cerr<<exc.what()<<std::endl;
	}
}

inline void charset_console_sample() throw(io::io_exception)
{
	// char set conversation sample
	uwriter out(io::Console::outChanell(), to_console_conv());
	out.write("Hello world English version. Привет мир, русская верссия\n\r");
	io::byte_buffer readBuff = io::new_byte_byffer(512);
	ureader in(io::Console::inChanell(),readBuff, from_console_conv());
	out.write("Type something :> ");
	out.write(in.read());
}

//inline void data_channel_sample() throw(io::io_exception)
//{
//	// data channel sample
//	char msg[14];
//	io::DataChannel dch((void*)msg, std::strlen(msg));
//	uwriter out(dch, to_console_conv());
//	out.write(TEXT("Hello from data channel\n"));
//}

inline void pipe_sample() throw(io::io_exception)
{
	// pipe sample
	io::SPipe pipe = io::open_pipe();
	boost::thread writeThread(boost::bind(pipe_write_routine,pipe));
	writeThread.start_thread();
	ureader reader(pipe->source(), io::new_byte_byffer(100), io::char_empty_converter());
	ustring str = reader.read();
	uwriter out(io::Console::outChanell(), to_console_conv());
	out.write(str);
}


#ifndef _MSC_VER
int main(int argc, const char** argv)
#else
int _tmain(int argc, TCHAR *argv[])
#endif
{
	try {
		change_console_charset();
		charset_console_sample();
//		data_channel_sample();
//		pipe_sample();
	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}

