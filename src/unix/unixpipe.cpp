#include "prchdrs.h"
#include "unixpipe.hpp"
#include "unixfile.hpp"
#include "helpers.hpp"

namespace io {

class UnixPipe:public Pipe
{
	public:
		UnixPipe(PipeSinkRoutine routine,int sink,int source);
		virtual SReadChannel source() const;
		virtual ~UnixPipe() BOOST_NOEXCEPT_OR_NOTHROW;
	private:
		static void write_routine(const UnixPipe* self, int hSink);
	private:
		int sink_;
		SReadChannel source_;
};

UnixPipe::UnixPipe(PipeSinkRoutine routine,int sink,int source):
	Pipe(routine),
	sink_(sink),
	source_(new FileChannel(source))
{
	boost::thread sinkThread(boost::bind(&UnixPipe::write_routine, this, sink));
}

void UnixPipe::write_routine(const UnixPipe* self, int hSink) {
	self->call_sink_routine(SWriteChannel(new FileChannel(hSink)));
}

SReadChannel UnixPipe::source() const
{
	return source_;
}

UnixPipe::~UnixPipe() BOOST_NOEXCEPT_OR_NOTHROW
{}

SPipe create_pipe(std::size_t buffer_size, PipeSinkRoutine routine)
{
	int files[2];
	::pipe(files);
	return SPipe(new UnixPipe(routine,files[1],files[0]));
}

} //namespace io
