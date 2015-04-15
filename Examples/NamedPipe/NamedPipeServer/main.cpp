#define CONV_ENGINE_ICONV

#include <console.hpp>
#include <file.hpp>
#include <namedpipe.hpp>
#include <text.hpp>

using namespace io;

const char* NAMED_PIPE_NAME = "TEST_NAMED_PIPE";

int main(int argc, const char** argv)
{
	Console con(true);
	u16writer info(con.out());
	try {
		SReadWriteChannel namedPipe = create_named_pipe(pipeName.c_str());
		SWriteChannel  out = boost::reinterpret_pointer_cast<WriteChannel>(namedPipe);
		u16writer owr(out);
		owr.writeln(u"Hello world!");
		owr.writeln(u"Привет вселенная!");
		owr.writeln(u"Γειά σου Κόσμε!");
	} catch(std::exception& exc) {
		writer ewr(con.err());
		ewr.write(exc.what());
	}
    return 0;
}
