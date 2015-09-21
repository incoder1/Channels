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

// Only to handle exception if anyassynch_experemental
#include <iostream>

#include <console.hpp>
#include <file.hpp>
#include <network.hpp>
#include <pipe.hpp>
#include <system.hpp>
#include <text.hpp>

void charset_console_sample() throw(io::io_exception)
{
	io::Console con(true);
	io::SConverter conv = io::make_converter(io::Charsets::utf8(),con.charset());
	io::cvt_writer out(con.out(),conv);
	out.writeln(io::cvt_writer::format("Hello! Привет! Χαιρετίσματα! %i") % 73);
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
		SWriteChannel fileChannel = file.openForWrite();
		writer out(fileChannel);
		uint8_t BOM[3] = {0xEF,0xBB,0xBF};
		out.flush(BOM, 3);
		out.write("Hello!\nПривет!\nΧαιρετίσματα!\nこんにちは!\n您好!");
	}
	SReadChannel in = file.openForRead();
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

void async_read_done(io::SAsyncChannel ch,const io::error_code& err,std::size_t read, io::byte_buffer& buff)
{
	io::Console con(false);
	con.setCharset(io::Charsets::utf8());
	if(!err) {
		buff.flip();
		io::writer out(con.out());
		out.flush(buff);
	} else {
		io::writer lerr(con.err());
		lerr.writeln(err.message());
	}
}

void async_write_done(io::SAsyncChannel ch,const io::error_code& err,std::size_t written, io::byte_buffer& buff) {
	io::Console con(false);
	con.setCharset(io::Charsets::utf8());
	if(!err) {
		io::writer linfo(con.out());
		linfo.writeln(boost::format("%i bytes written") % written);
		ch->receive(0, written, io::AsyncChannel::handler_f(async_read_done) );
	} else {
		io::writer lerr(con.err());
		lerr.writeln(err.message());
	}
}

void async_file_sample() {
	using namespace io;
	File file("asynch_result.txt");
	if(file.exist()) {
		if(!file.remove()) {
			boost::throw_exception(io_exception("Can not delete result.txt"));
		}
	}
	file.create();

	boost::asio::io_service ios;
    //boost::asio::io_service::work work(ios);

	SAsyncChannel ch = file.openAsynchronous(ios);

	char msg[75] = "   Hello!\nПривет!\nΧαιρετίσματα!\nこんにちは!\n您好!";

	msg[0]=0xEF; msg[1]=0xBB; msg[2] = 0xBF;
	byte_buffer buff = byte_buffer::copy_array(msg,74);
	ch->send(0, buff, AsyncChannel::handler_f(async_write_done) );

	boost::thread listenThread(boost::bind(&boost::asio::io_service::run, &ios));
	listenThread.join();
}



void buffers_sample()
{
	using namespace io;
	const char* hello = "Hello ";
	const char* world = "world!!!";

	byte_buffer result = byte_buffer::heap_buffer(14);

	const byte_buffer wrap = byte_buffer::wrap_array(hello, 6); //not wrap the 0 ending
	byte_buffer deepCopy = byte_buffer::copy_array(world, 8); // copy 0 also

	// resize and add 0-ro string ending
	deepCopy = deepCopy.resize(9);
	deepCopy.move(8);
	deepCopy.put(0);
	deepCopy.flip(); // flips the buffer

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

void network_client_sample() {

	using namespace boost::asio;

	io::Console con(false);
	con.setCharset(io::Charsets::utf8());
	io::writer out(con.out());

	io_service service;
	ip::tcp::resolver resolver(service);
	ip::tcp::resolver::query query("www.google.com", "80");
	ip::tcp::resolver::iterator iter = resolver.resolve(query);
	boost::asio::ip::tcp::endpoint ep = *iter;

	service.run();

	boost::shared_ptr<ip::tcp::socket> socket = boost::make_shared<ip::tcp::socket>(service);
	socket->open(ep.protocol());
	socket->connect(ep);
	out.writeln(boost::format("Connecting to: %s %i / %i") % ep.address().to_string() % ep.port() % ep.protocol().family());
	io::net::STCPSocketChannel socketChannel( new io::net::TCPSocketChannel(socket) );

	const char* REQ = "HEAD / HTTP/1.1\r\nHost:www.google.com\r\nAccept: */*\r\nUser-Agent:Channels C++ IO library\r\nAccept-Charset:ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\nPragma: no-cache\n\rCache-Control: no-cache\n\rConnection: close\r\n\r\n";
	out.writeln(boost::format("HTTP request: %s \n") % REQ);
	io::writer request(socketChannel);
	request.write(REQ);

	out.writeln("HTTP response:");
	io::byte_buffer buffer = io::byte_buffer::heap_buffer(128);
	if(socketChannel->read(buffer) > 0) {
		buffer.flip();
		out.flush(buffer);
		buffer.clear();
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
		//charset_console_sample();
		async_file_sample();
		//pipe_sample();
		//file_sample();
		//asynch_file_sample();
		//network_client_sample();
	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}

