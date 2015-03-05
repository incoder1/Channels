#include <cstax.hpp>
#include <file.hpp>

#include <iostream>

int main(int argc,const char **argv) {
	io::File file("test.xml");
	if(!file.exist()) {
		std::cerr<<"no input file"<<std::endl;
		return -1;
	}
	using namespace xml;
	Source source = Source::create(file.openForRead(),"UCS-2");
	StreamReader reader(source);
	while(reader.hasNext()) {
		SEvent event = reader.next();
		switch( event->type() ) {
		case START_DOCUMENT:
			SDocumentEvent ev = boost::static_pointer_cast<DocumentEvent,Event>(event);
			std::cout<<"version:"<<ev->version()<<" encoding:"<<ev->encoding()<<" standalone:"<<ev->isStandalone()<<std::endl;
			return 0;
		}
	}
	return 0;
}
