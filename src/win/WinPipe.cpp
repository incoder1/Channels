#include "prchdrs.h"
#include "helpers.hpp"

#include <boost/thread/scoped_thread.hpp>
#include <boost/make_shared.hpp>

#include "pipe.hpp"
#include "WinFile.hpp"

namespace io {

class WindowsPipe:public Pipe {
public:
	WindowsPipe(PipeSinkRoutine routine,HANDLE sink,HANDLE source);
	virtual ~WindowsPipe() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual SReadChannel source() const;
private:
	static void write_routine(const WindowsPipe* self, HANDLE hSink);
private:
	SReadChannel source_;
};

WindowsPipe::WindowsPipe(PipeSinkRoutine routine,HANDLE sink,HANDLE source):
	 Pipe(routine),
     source_(new WinChannel(source,GENERIC_READ))
{
	boost::thread sinkThread(boost::bind(&WindowsPipe::write_routine, this, sink));
}

void WindowsPipe::write_routine(const WindowsPipe* self, HANDLE hSink) {
	self->call_sink_routine(SWriteChannel(new WinChannel(hSink, GENERIC_WRITE)));
}

WindowsPipe::~WindowsPipe() {
}

SReadChannel WindowsPipe::source() const {
	return source_;
}

SPipe CHANNEL_PUBLIC create_pipe(std::size_t bufferSize, PipeSinkRoutine sinkRoutine)
{
	::HANDLE sink, source = NULL;
	validate_io(::CreatePipe(&source, &sink, NULL, DWORD(bufferSize)), "Can not create anonymous pipe");
	return SPipe(new WindowsPipe(sinkRoutine,sink,source));
}

} // namesapce io
