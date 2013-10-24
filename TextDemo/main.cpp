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

//		// OK lets try to use some network
//		// FIXME create dedicated demo for the network
//		using boost::asio::ip::tcp;
//
//		boost::asio::io_service io_service;
//
//		// Get a list of endpoints corresponding to the server name.
//		tcp::resolver resolver(io_service);
//		tcp::resolver::query query("google.com", "http");
//		tcp::resolver::iterator epIT = resolver.resolve(query);
//
//		// Try each endpoint until we successfully establish a connection.
//		boost::shared_ptr<tcp::socket> socket(new tcp::socket(io_service));
//		boost::asio::connect(*socket, epIT);
//
//		io::network::TCPSocketChannel *httpch = new io::network::TCPSocketChannel(socket);
//		io::PWriteChannel rqCh(httpch);
//		//io::PReadChannel rspCh(httpch, no_delete_free);
//
//		uwriter req(rqCh,io::new_converter(UTF16,"UTF-8"));
//		req.write(L"GET / HTTP/1.0\r\nHOST google.com\r\nAccept: */*\r\nConnection: close\r\n\r\n");
//
//		io::byte_buffer respBytes = io::new_byte_byffer(1 << 20);
//		httpch->read(respBytes);
//		respBytes.flip();
//		// write results into console
//		io::Console::outChanell()->write(respBytes);
//
//		//ureader resp();

	} catch(std::exception &e) {
		std::cerr<<e.what()<<std::endl;
	}

	return 0;
}
