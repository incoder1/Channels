#include <cstax.hpp>
#include <file.hpp>

#include <iostream>
#include <console.hpp>

int main(int argc,const char **argv)
{
	io::Console console(false);
	console.setCharset(io::Charsets::forName("UTF-8"));
	//console.setColor(io::LIGHT_YELLOW, io::LIGHT_BLUE);
	io::writer out(console.out());

	io::File file("test.xml");
	if(!file.exist()) {
		std::cerr<<"no input file"<<std::endl;
		return -1;
	}
	using namespace xml;
	SSource source = make_source(file.openForRead(),256);
	StreamReader reader(source);
	while(reader.hasNext()) {
		SEvent event = reader.next();
		switch(event->type()) {
			case START_DOCUMENT: {
				auto dev = up_cast<SDocumentEvent>(event);
				std::cout<<"Version: "<<dev->version()<<" Encoding: "<<dev->encoding()<<" Standalone:"<<dev->standalone()<<std::endl;
			}
			break;
		case PROCESSING_INSTRUCTION: {
			auto piev = up_cast<SProcessingInstructionEvent>(event);
			std::cout<<"Type: "<<piev->instType()<<" Href: "<<piev->href()<<std::endl;
			}
			break;
		}
	}
	return 0;
}
