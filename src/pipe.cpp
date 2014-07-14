#include "prchdrs.h"
#include "pipe.hpp"

namespace io {

namespace _pipe {

PipeChannel::PipeChannel(byte_buffer source) BOOST_NOEXCEPT_OR_NOTHROW:
		ReadChannel(),
		WriteChannel(),
		source_(source),
		mutex_(),
		condition_(),
		canRead_(false)
{}


std::size_t PipeChannel::read(byte_buffer& buffer) throw(io_exception) {
	boost::shared_lock<boost::shared_mutex> lock(mutex_);
	while(!canRead_) {
		condition_.wait(lock);
	}
	std::size_t result = buffer.put(source_);
	source_.move(result);
	return result;
}

std::size_t PipeChannel::write(const byte_buffer& buffer) throw(io_exception)
{
	boost::upgrade_lock<boost::shared_mutex> lock(mutex_);
	boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
	source_.clear();
	std::size_t result = source_.put(buffer);
	source_.flip();
	canRead_ = true;
	condition_.notify_all();
	return result;
}

} // namespace _pipe

//Windows Pipe
Pipe::Pipe(const byte_buffer& buffer) throw(std::bad_alloc):
	pipeCh_(new _pipe::PipeChannel(buffer))
{
}

SWriteChannel Pipe::sink() BOOST_NOEXCEPT_OR_NOTHROW {
	boost::shared_ptr<_pipe::PipeChannel> result(pipeCh_);
	return boost::dynamic_pointer_cast<WriteChannel>(result);
}

SReadChannel Pipe::source() BOOST_NOEXCEPT_OR_NOTHROW {
	boost::shared_ptr<_pipe::PipeChannel> result(pipeCh_);
	return boost::dynamic_pointer_cast<ReadChannel>(result);
}

} // namespace io
