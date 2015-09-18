#include <cstax.hpp>
#include <file.hpp>
#include <console.hpp>

int main(int argc,const char **argv)
{
	io::Console console(false);
	console.setCharset(io::Charsets::forName("UTF-8"));
	//console.setColor(io::LIGHT_YELLOW, io::LIGHT_BLUE);
	io::writer out(console.out());

	io::File file("test.xml");
	if(!file.exist()) {
		io::writer err(console.err());
		err.writeln("no input file");
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
				out.writeln(boost::format("Version:%s Encoding:%s Standalone:%i") % dev->version() % dev->encoding() % dev->standalone());
			}
			break;
		case PROCESSING_INSTRUCTION: {
			auto piev = up_cast<SProcessingInstructionEvent>(event);
			out.writeln(boost::format("Type:%s Href:%s") % piev->instType() % piev->href());
			}
			break;
		}
	}
	return 0;
}
