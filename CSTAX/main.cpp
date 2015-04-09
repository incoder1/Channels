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
	SSource source = make_source(file.openForRead(),io::Charsets::forName("UCS-2"),128);
	StreamReader reader(source);
	//while(reader.hasNext()) {
		SEvent event = reader.next();
		auto dev = up_cast<SDocumentEvent>(event);
		std::cout<<"Version: "<<dev->version()<<" Encoding: "<<dev->encoding()<<" Standalone:"<<dev->standalone()<<std::endl;
	//}
	return 0;
}
