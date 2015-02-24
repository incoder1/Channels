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
#include <text.hpp>
#include <pipe.hpp>
#include <network.hpp>

#include <boost/thread/thread.hpp>

#ifndef HAS_CPP11
	typedef unsigned short char16_t;
	typedef char char8_t;
#endif

// wide string string
typedef std::basic_string<TCHAR> wstring;

#if	defined(PLATFORM_WINDOWS)
// Windows NT Unicode console - UTF-16LE
const char* LOCALE_CH = "UTF-16LE";

inline io::SConverter to_console_conv() {
	return io::new_converter("UTF-8",LOCALE_CH);
}

#elif defined(PLATFROM_UNIX)

inline io::SConverter to_console_conv() {
	return io::char_empty_converter();
}

#else
#	error "This operating system is not supported yet"
#endif


typedef io::Writer<std::string> writer_u8;
typedef io::Reader<std::string> reader_u8;

typedef io::Writer<wstring> wwriter;
typedef io::Reader<wstring> wreader;

void charset_console_sample() throw(io::io_exception)
{
	io::Console con(true);
	con.setCharset(LOCALE_CH);
	writer_u8 out(con.outChanell(), to_console_conv());
	out.write("Hello! Привет! Χαιρετίσματα! こんにちは! 您好！ \n\r");
	out.write("If you can't see your language please change console font");
}

void pipe_write_routine(io::SWriteChannel sink) {
	writer_u8 pout(sink, io::char_empty_converter());
	pout.write("Hello from pipe!\n");
	pout.write("\tПривет из канала!\n");
	//pout.write("\tΧαιρετίσματα από το κανάλι!\n");
	pout.write("\tGrüße aus dem Kanal!\n");
}


void pipe_sample()
{
	// pipe sample
	io::SPipe pipe = io::create_pipe(100, io::PipeSinkRoutine(pipe_write_routine));
	io::byte_buffer readbuf = io::byte_buffer::heap_buffer(80);
	io::Console con(true);
	wwriter cwr(con.outChanell(), to_console_conv());
	while( 0 < pipe->source()->read(readbuf) ) {
		readbuf.flip();
		cwr.write(readbuf);
	}
}

/**
 * Writes wchar_t string into UTF-8 file
 */
void file_sample() {
	using namespace io;
	File file("result.txt");
	if(file.exist()) {
		if(!file.errace()) {
			boost::throw_exception(io_exception("Can not delete result.txt"));
		}
	}
	file.create();
	auto fileChannel = file.openForWrite();
	uint8_t bom[2] = {0xFF,0xFE};
	const io::byte_buffer buff = io::byte_buffer::wrap_array(bom,2);
	fileChannel->write(buff);
	writer_u8 out(fileChannel, io::new_converter("UTF-8","UTF-16LE"));
	out.write("ASCII     abcde xyz\n\rGerman  äöü ÄÖÜ ß\n\rPolish  ąęźżńł\n\rRussian  абвгдеж эюя\n\rCJK  你好\n\r");
}

void buffers_sample() {
	using namespace io;
	const char* hello = "Hello ";
	const char* world = "world!!!";

	byte_buffer result = byte_buffer::heap_buffer(15);

	const byte_buffer wrap = byte_buffer::wrap_array(hello, 6); //not wrap the 0 ending
	byte_buffer deepCopy = byte_buffer::copy_array(world, 9); // copy 0 also
	byte_buffer wrapDeepCopy(wrap);

	result.put(wrapDeepCopy);
	result.put(deepCopy);
	result.flip();

	std::cout<<"Length:"<<result.length()<<" "<<result.position().ptr()<<std::endl;
}

void network_client_sample() {

	using namespace boost::asio;

	io::SConverter conv = to_console_conv();

	io::Console con(true);
	con.setCharset(LOCALE_CH);

	io_service service;
	ip::tcp::resolver resolver(service);
	ip::tcp::resolver::query query("www.google.com", "80");
	ip::tcp::resolver::iterator iter = resolver.resolve(query);
	boost::asio::ip::tcp::endpoint ep = *iter;
	service.run();

	boost::shared_ptr<ip::tcp::socket> sock(new ip::tcp::socket(service) );
	sock->open(ep.protocol());
	sock->connect(ep);
	std::cout<<"Connecting to:"<< ep.address().to_string() << ":" << ep.port() << "/" << ep.protocol().family() << std::endl;

	io::net::STCPSocketCahnnel netCh( new io::net::TCPSocketChannel(sock) );

	const char* REQ = "GET / HTTP/1.1\r\nHost:www.google.com\r\nAccept: */*\r\nUser-Agent:Channels C++ IO library\r\nAccept-Charset:ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\nPragma: no-cache\n\rCache-Control: no-cache\n\rConnection: close\r\n\r\n";
	std::cout<<"REQUEST:"<<REQ<<std::endl<<std::endl;
	netCh->write(io::byte_buffer::wrap_str(REQ));

	// Character conversation from UTF-8 to console, in case of windows - UTF16-LE
	auto readb = io::byte_buffer::heap_buffer(128);
	auto convb = io::byte_buffer::heap_buffer(256);
	while(netCh->read(readb) > 0) {
		readb.flip();
		conv->convert(readb,convb);
		convb.flip();
		// write to console
		con.outChanell()->write(convb);
		convb.clear();
		readb.clear();
	}

}

#ifndef _MSC_VER
int main(int argc, const char** argv)
#else
int _tmain(int argc, TCHAR *argv[])
#endif
{
	try {
		//buffers_sample();
		charset_console_sample();
		//pipe_sample();
		//file_sample();
		//network_client_sample();
	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}

