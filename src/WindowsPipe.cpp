#include "prchdrs.h"
#include "WindowsPipe.hpp"

namespace io {

//Windows Pipe
WindowsPipe::WindowsPipe() throw(io_exception):
     mutex_(),
     condition_(),
     sink_(NULL),
     source_(NULL),
     canRead_(false)
{
	// No inheritance and default system buffer size
	if(!::CreatePipe(&source_, &sink_, NULL, 0)) {
		boost::throw_exception(io_exception("Can not create anonymous pipe"));
	}
}

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
