#include <cstax.hpp>
#include <file.hpp>

#include <iostream>
#include <console.hpp>

int main(int argc,const char **argv)
{
	try {
		io::File file("test.xml");
		if(!file.exist()) {
			std::cerr<<"no input file"<<std::endl;
			return -1;
		}
		io::Console console(false);
		console.setCharset(io::Charsets::forName("UTF-8"));
		//console.setColor(io::LIGHT_YELLOW, io::LIGHT_BLUE);
		io::writer wr(console.out());

		using namespace xml;
			SSource source = make_source(file.openForRead(),io::Charsets::forName("UCS-2"),128);
			StreamReader reader(source);
			while(reader.hasNext()) {
				SEvent event = reader.next();
				switch( event->type() ) {
				case START_DOCUMENT:
					SDocumentEvent ev = event_up_cast<SDocumentEvent>(event);
					std::cout<<"version:"<<ev->version()<<" encoding:"<<ev->encoding()<<" standalone:"<<ev->isStandalone()<<std::endl;
					return 0;
				}
			}
	} catch(std::exception& exc) {
		std::cerr<<exc.what()<<std::endl;
	}
	return 0;
}
