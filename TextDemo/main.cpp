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

//#include <network.hpp>

#ifndef HAS_CPP11
	typedef unsigned short char16_t;
	typedef char char8_t;
#endif

// UTF8 string
typedef std::basic_string<TCHAR> wstring;
// UTF16 string
typedef std::string ustring;

#if	defined(PLATFROM_WINDOWS)
// Windows NT Unicode locale - UTF16-LE representation
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

void change_console_charset()
{
	io::Console::setCharset(io::Charsets::forName(LOCALE_CH));
}

void pipe_write_routine(io::SPipe pipe) throw(io::io_exception)
{
	try {
		writer_u8 out(pipe->sink(),io::char_empty_converter());
		out.write(U8("Hello from Pipe"));
	} catch(std::exception& exc) {
		std::cerr<<exc.what()<<std::endl;
	}
}

void charset_console_sample() throw(io::io_exception)
{
	// char set conversation sample
	writer_u8 out(io::Console::outChanell(), to_console_conv());
	out.write(U8("Hello world English version. Привет мир, русская верссия\n\r"));
	io::byte_buffer readBuff = io::new_byte_byffer(512);
	reader_u8 in(io::Console::inChanell(),readBuff, from_console_conv());
	out.write(U8("Type something :> "));
	out.write(in.read());
}

void pipe_sample() throw(io::io_exception)
{
	// pipe sample
	io::SPipe pipe = io::open_pipe();
	boost::thread writeThread(boost::bind(pipe_write_routine,pipe));
	writeThread.start_thread();
	reader_u16le reader(pipe->source(), io::new_byte_byffer(100), to_console_conv());
	wstring str = reader.read();
	writer_u16le out(io::Console::outChanell(), io::char_empty_converter());
	out.write(str);
}

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

class MyAsynchReadCH: public io::AsynchronousCompletionHandler {
public:
	MyAsynchReadCH():
		AsynchronousCompletionHandler(),
		mutex_(),
		condition_()
	{}
	void success(const io::byte_buffer& buff,std::size_t size) {
		std::cout<<"hello"<<std::endl;
		writer_u8 out(io::Console::outChanell(), to_console_conv());
		out.write(buff);
		boost::unique_lock<boost::mutex> lock(mutex_);
		condition_.notify_all();
	}
	void failed(const char* errorMessage) {
	}
	void await() {
		boost::unique_lock<boost::mutex> lock(mutex_);
		condition_.wait(lock);
	}
private:
	boost::mutex mutex_;
	boost::condition_variable_any condition_;
};

void asynch_read_routine(io::SAsynchReadChannel ch,const io::byte_buffer& buff, io::SAsynchCompletionHandler handler) {
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
		//pipe_sample();
		file_sample();
	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}

