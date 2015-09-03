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


#include <win\WinAssynchChannel.hpp>

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

void handle_asynch_io(const boost::system::error_code& ec, std::size_t transfered,const io::byte_buffer& buffer) {
	std::cout<<"Transfered="<<transfered<<std::endl;
}

static std::string last_error_str(DWORD lastError)
{
	std::string result;
	LPVOID lpMsgBuf;
	static DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD bufLen = ::FormatMessageA(flags,NULL,lastError,
	                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	                   (LPSTR) &lpMsgBuf, 0, NULL );
	if (bufLen) {
		LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
		result.append(lpMsgStr, lpMsgStr+bufLen);
		::LocalFree(lpMsgBuf);
	}
	return result;
}

void asynh_file_sample() {
	io::WinSelector selector(0);
	io::File file("assynh_text.txt");
	if(file.exist()) {
		if(!file.remove()) {
			boost::throw_exception(io::io_exception("Can not delete assynh_text.txt"));
		}
	}
	::HANDLE hFile = ::CreateFile(L"assynh_text.txt", GENERIC_READ | GENERIC_WRITE , 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
	if(INVALID_HANDLE_VALUE == hFile) {
		boost::throw_exception(std::runtime_error(last_error_str(::GetLastError())));
	}
	io::WinAsynchChannel winAsynchChannel(&selector,hFile,io::completition_handler_f(handle_asynch_io));
	selector.start();
	winAsynchChannel.send(0,io::byte_buffer::wrap_array("Hello world!",12));
}


void buffers_sample()
{
	using namespace io;
	const char* hello = "Hello ";
	const char* world = "world!!!";

	byte_buffer result = byte_buffer::heap_buffer(14);

	const byte_buffer wrap = byte_buffer::wrap_array(hello, 6); //not wrap the 0 ending
	byte_buffer deepCopy = byte_buffer::copy_array(world, 8); // copy 0 also

	// resize and add 0-ro sting ending
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
		//pipe_sample();
		//file_sample();
		asynh_file_sample();
		//asynch_file_sample();
		//network_client_sample();
	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}
	return 0;
}

