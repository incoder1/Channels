#include "prchdrs.h"
#include "helpers.hpp"

#include  <boost/thread/scoped_thread.hpp>

#include  <pipe.hpp>
#include  <WindowsFile.hpp>

namespace io {

class WindowsPipe:public Pipe {
public:
	WindowsPipe(PipeSinkRoutine routine,HANDLE sink,HANDLE source);
	virtual ~WindowsPipe() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual SReadChannel source() const;
private:
	static void write_routine(const PipeSinkRoutine routine, HANDLE hSink);
private:
	SReadChannel source_;
};

WindowsPipe::WindowsPipe(PipeSinkRoutine routine,HANDLE sink,HANDLE source):
	 Pipe(routine),
     source_(new FileChannel(source,true))
{
	boost::thread sinkThread(boost::bind(&WindowsPipe::write_routine, routine, sink));
	boost::strict_scoped_thread<> s(boost::move(sinkThread));
	sinkThread.detach();
}

void WindowsPipe::write_routine(const PipeSinkRoutine routine, HANDLE hSink) {
	routine(SWriteChannel(new FileChannel(hSink, true)));
}

WindowsPipe::~WindowsPipe() {
}

SReadChannel WindowsPipe::source() const {
	return source_;
}

SPipe CHANNEL_PUBLIC create_pipe(std::size_t bufferSize, PipeSinkRoutine sinkRoutine) throw(io_exception)
{
	HANDLE sink, source = NULL;
	validate_io(::CreatePipe(&source, &sink, NULL, DWORD(bufferSize)), "Can not create anonymous pipe");
	return SPipe(new WindowsPipe(sinkRoutine,sink,source));
}

} // namesapce io
