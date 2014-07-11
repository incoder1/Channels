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
	io::byte_buffer readBuff = io::byte_buffer::new_heap_buffer(512);
	reader_u8 in(con.inChanell(),readBuff, from_console_conv());
	out.write(U8("Type something :> "));
	out.write(in.read());
}

void pipe_write_routine(io::SWriteChannel sink) {
	try {
		sink->write(io::byte_buffer::wrap_array("Hello from Pipe",15));
	} catch(std::exception& exc) {
		std::cerr<<exc.what()<<std::endl;
	}
}


void pipe_sample()
{
	// pipe sample
	io::Pipe pipe(io::byte_buffer::new_heap_buffer(30));
	boost::thread writeThread(boost::bind(pipe_write_routine,pipe.sink()));
	writeThread.start_thread();
	reader_u16le reader(pipe.source(), io::byte_buffer::new_heap_buffer(100), to_console_conv());
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

void buffers_smaple() {
	using namespace io;
	const char* hello = "Hello ";
	const char* world = "world!!!";

	byte_buffer result = byte_buffer::new_heap_buffer(std::strlen("Hello world!!!"));

	byte_buffer wrap = byte_buffer::wrap_array(hello, std::strlen(hello)-1); //not wrap the 0 ending
	byte_buffer deepCopy = byte_buffer::copy_array(world, std::strlen(world)+1); // copy 0 also

	result.put(wrap.position(), wrap.last());
	result.put(deepCopy.position(), deepCopy.last());
	result.flip();
	std::cout<<"Buffer length: "<<result.length()<<" Buffer content: "<<result.position().ptr()<<std::endl;
}

#ifndef _MSC_VER
int main(int argc, const char** argv)
#else
int _tmain(int argc, TCHAR *argv[])
#endif
{
	try {
		buffers_smaple();
		//charset_console_sample();
		//pipe_sample();
		//file_sample();
	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}

