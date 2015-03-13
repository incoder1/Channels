#include <cstax.hpp>
#include <file.hpp>

#include <iostream>
#include <console.hpp>

int main(int argc,const char **argv) {
	io::File file("test.xml");
	if(!file.exist()) {
		std::cerr<<"no input file"<<std::endl;
		return -1;
	}
	io::Console console(false);
	console.setCharset(io::Charsets::forName("UTF-8"));
	console.setColor(io::LIGHT_YELLOW, io::LIGHT_BLUE);
	io::Writer<std::string> wr(console.outChanell(), io::char_empty_converter());

	using namespace xml;
	Source source = Source::create(file.openForRead(),"UCS-2");
	StreamReader reader(source);
	while(reader.hasNext()) {
		SEvent event = reader.next();
		switch( event->type() ) {
		case START_DOCUMENT:
			wr.write("есть текст на русском\n");
			SDocumentEvent ev = boost::static_pointer_cast<DocumentEvent,Event>(event);
			std::cout<<"version:"<<ev->version()<<" encoding:"<<ev->encoding()<<" standalone:"<<ev->isStandalone()<<std::endl;
			return 0;
		}
	}
	return 0;
}
