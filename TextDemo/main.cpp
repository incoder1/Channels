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

#ifdef HAS_CPP11
#	define U8(str) u8##str
#else
#	define U8(str) str
#endif // HAS_CPP11

// Only to handle exception if any
#include <iostream>
#include <console.hpp>
#include <readwrite.hpp>
#include <datachannel.hpp>
#include <pipe.hpp>

#include <boost/thread/thread.hpp>

//#include <network.hpp>

#ifndef HAS_CPP11
	typedef unsigned short char16_t;
	typedef char char8_t;
#endif

// wide string string
typedef std::basic_string<TCHAR> wstring;
// UTF8 string
typedef std::string ustring;

#if	defined(PLATFORM_WINDOWS)
// Windows NT Unicode console - UTF-16LE
const char* LOCALE_CH = "UTF-16LE";

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


typedef io::Writer<ustring> writer_u8;
typedef io::Reader<ustring> reader_u8;

typedef io::Writer<wstring> writer_u16le;
typedef io::Reader<wstring> reader_u16le;

void charset_console_sample() throw(io::io_exception)
{
	const io::Charset* utf16 = io::Charsets::forName(LOCALE_CH);
	io::Console con(true);
	con.setCharset(utf16);
	// char set conversation sample
	writer_u8 out(con.outChanell(), to_console_conv());
	out.write(U8("Hello world English version. Привет мир, русская верссия\n\r"));
	io::byte_buffer readBuff = io::new_byte_byffer(512);
	reader_u8 in(con.inChanell(),readBuff, from_console_conv());
	out.write(U8("Type something :> "));
	out.write(in.read());
}

void pipe_write_routine(io::SWriteChannel sink) throw(io::io_exception)
{
	try {
		writer_u8 out(sink,io::char_empty_converter());
		out.write(U8("Hello from Pipe"));
	} catch(std::exception& exc) {
		std::cerr<<exc.what()<<std::endl;
	}
}


void pipe_sample() throw(io::io_exception)
{
	// pipe sample
	io::Pipe pipe;
	boost::thread writeThread(boost::bind(pipe_write_routine,pipe.sink()));
	writeThread.start_thread();
	reader_u16le reader(pipe.source(), io::new_byte_byffer(100), to_console_conv());
	wstring str = reader.read();
	io::Console con;
	writer_u16le out(con.outChanell(), io::char_empty_converter());
	out.write(str);
}

/**
 * Writes wchar_t string into UTF-8 file
 */
void file_sample() {
	using namespace io;
	File file("sample.txt");
	if(file.exist()) {
		if(!file.errace()) {
			boost::throw_exception(io_exception("Can not delete sample.txt"));
		}
	}
	file.create();
	writer_u16le out(file.openForWrite(), io::new_converter(LOCALE_CH,"UTF-8"));
	out.write(L"ASCII     abcde xyz\nGerman  äöü ÄÖÜ ß\nPolish  ąęźżńł\nRussian  абвгдеж эюя\nCJK  你好");
}

#ifndef _MSC_VER
int main(int argc, const char** argv)
#else
int _tmain(int argc, TCHAR *argv[])
#endif
{
	try {
		charset_console_sample();
		//pipe_sample();
		//file_sample();
	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}

