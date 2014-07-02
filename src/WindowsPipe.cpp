#include "pipe.hpp"

namespace io {

// Abstract Pipe
Pipe::Pipe() BOOST_NOEXCEPT_OR_NOTHROW
{}

Pipe::~Pipe() BOOST_NOEXCEPT_OR_NOTHROW
{}

// factory function
SPipe CHANNEL_PUBLIC open_pipe() throw(io_exception)
{
	HANDLE sync;
	HANDLE src;
	// No inheritance and default system buffer size
	if(!::CreatePipe(&src, &sync, NULL, 0)) {
		boost::throw_exception(io_exception("Can not create anonymous pipe"));
	}
	return SPipe(new WindowsPipe(sync,src));
}

//Windows Pipe

WindowsPipe::WindowsPipe(HANDLE sink, HANDLE source) BOOST_NOEXCEPT_OR_NOTHROW:
Pipe(),
     mutex_(),
     condition_(),
     canRead_(false),
     sink_(sink),
     source_(source)
{}

WindowsPipe::~WindowsPipe() BOOST_NOEXCEPT_OR_NOTHROW
{}

SWriteChannel WindowsPipe::sink() BOOST_NOEXCEPT_OR_NOTHROW {
	lock_t lock(mutex_);
	SWriteChannel result(new FileChannel(sink_, true));
	canRead_ = true;
	condition_.notify_all();
	return result;
}

SReadChannel WindowsPipe::source() BOOST_NOEXCEPT_OR_NOTHROW {
	while(!canRead_) {
		lock_t lock(mutex_);
		condition_.wait(lock);
	}
	return SReadChannel(new FileChannel(source_, true));
}

} // namespace io
