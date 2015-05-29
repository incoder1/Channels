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
#include <file.hpp>
#include <pipe.hpp>
#include <network.hpp>

#include <win/WinAssynchChannel.hpp>

void charset_console_sample() throw(io::io_exception)
{
	io::Console con(true);
	io::SConverter conv = io::make_converter(io::Charsets::utf8(),con.charset());
	io::cvt_writer out(con.out(),conv);
	out.writeln(boost::format("Hello! Привет! Χαιρετίσματα! %i") % 73);
}

void pipe_write_routine(io::SWriteChannel sink)
{
	io::wwriter pout(sink);
	pout.write(L"Hello from pipe!\n");
	pout.write(L"Привет из канала!\n");
	pout.write(L"Χαιρετίσματα από το κανάλι!\n");
	pout.write(L"Grüße aus dem Kanal!\n");
}


void pipe_sample()
{
	// pipe sample
	io::SPipe pipe = io::create_pipe(100, io::PipeSinkRoutine(pipe_write_routine));
	io::byte_buffer readbuf = io::byte_buffer::heap_buffer(80);
	io::Console con(true);
	while( 0 < pipe->source()->read(readbuf) ) {
		readbuf.flip();
		con.out()->write(readbuf);
		readbuf.clear();
	}
}

/**
 * Writes wchar_t string into UTF-8 file
 */
void file_sample()
{
	using namespace io;
	File file("result.txt");
	if(file.exist()) {
		if(!file.remove()) {
			boost::throw_exception(io_exception("Can not delete result.txt"));
		}
	}
	file.create();
	{
		auto fileChannel = file.openForWrite();
		writer out(fileChannel);
		uint8_t BOM[3] = {0xEF,0xBB,0xBF};
		out.flush(BOM, 3);
		out.write("Hello!\nПривет!\nΧαιρετίσματα!\nこんにちは!\n您好!");
	}
	auto in = file.openForRead();
	Console con(true);
	wwriter out(con.out());
	byte_buffer buff = byte_buffer::heap_buffer(128);
	cvt_reader reader( in, make_converter(Charsets::utf8(),con.charset()) );
	std::size_t converted = reader.read(buff);
	while(converted > 0) {
		out.flush(buff);
		buff.clear();
		converted = reader.read(buff);
	}
}

void asynch_file_sample()
{
	io::Selector selector;
	io::SAsynchDevice device = io::AsynchDevice::file("asynch.txt");
}

void buffers_sample()
{
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

	result = result.clone();

	Console con(false);
	con.setCharset(Charsets::utf8());
	writer out(con.out());
	out.write(boost::format("Buffer Length: %i\nBuffer Value: %s\n") % result.length() % result.position().ptr() );
}

//void network_client_sample() {
//
//	using namespace boost::asio;
//
//	io::SConverter conv = io::new_converter("UTF-8","UTF-16LE");
//
//	io::Console con(true);
//
//	io_service service;
//	ip::tcp::resolver resolver(service);
//	ip::tcp::resolver::query query("www.google.com", "80");
//	ip::tcp::resolver::iterator iter = resolver.resolve(query);
//	boost::asio::ip::tcp::endpoint ep = *iter;
//	service.run();
//
//	boost::shared_ptr<ip::tcp::socket> sock(new ip::tcp::socket(service) );
//	sock->open(ep.protocol());
//	sock->connect(ep);
//	std::cout<<"Connecting to:"<< ep.address().to_string() << ":" << ep.port() << "/" << ep.protocol().family() << std::endl;
//
//	io::net::STCPSocketCahnnel netCh( new io::net::TCPSocketChannel(sock) );
//
//	const char* REQ = "HEAD / HTTP/1.1\r\nHost:www.google.com\r\nAccept: */*\r\nUser-Agent:Channels C++ IO library\r\nAccept-Charset:ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\nPragma: no-cache\n\rCache-Control: no-cache\n\rConnection: close\r\n\r\n";
//	std::cout<<"REQUEST:"<<REQ<<std::endl<<std::endl;
//	netCh->write(io::byte_buffer::wrap_str(REQ));
//
//	// Character conversation from UTF-8 to console, in case of windows - UTF16-LE
//	auto readb = io::byte_buffer::heap_buffer(128);
//	auto convb = io::byte_buffer::heap_buffer(256);
//	while(netCh->read(readb) > 0) {
//		readb.flip();
//		conv->convert(readb,convb);
//		convb.flip();
//		// write to console
//		con.outChanell()->write(convb);
//		convb.clear();
//		readb.clear();
//	}
//
//}

#ifndef _MSC_VER
int main(int argc, const char** argv)
#else
int _tmain(int argc, TCHAR *argv[])
#endif
{
	try {
		buffers_sample();
		//charset_console_sample();
		//pipe_sample();
		//file_sample();
		//asynch_file_sample();
		//network_client_sample();
	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}

