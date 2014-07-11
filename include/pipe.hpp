#ifndef CHANNEL_PIPE_HPP_INCLUDED
#define CHANNEL_PIPE_HPP_INCLUDED

// boost imports
#include <boost/weak_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

// modules imports
#include <channels.hpp>

namespace io {

// private implementation, not visible outside of library
namespace _pipe {

class PipeChannel:public virtual ReadChannel, public virtual WriteChannel, public virtual SmallObject {
private:
	typedef boost::unique_lock<boost::mutex> lock_t;
public:
	explicit PipeChannel(byte_buffer source) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t read(byte_buffer& buffer) throw(io_exception);
	virtual std::size_t write(const byte_buffer& buffer) throw(io_exception);
private:
	byte_buffer source_;
	boost::mutex mutex_;
	boost::condition_variable_any condition_;
	bool volatile canRead_;
};

} // namespace _pipe

/**
* ! \brief  A pair of channels that implements a unidirectional pipe.
*
* <p> A pipe consists of a pair of channels: A writable sink channel and a readable source channel.
* Once some bytes are written to the sink channel they can be read from source channel in exactly
* the order in which they were written.
*
* <p> Whether or not a thread writing bytes to a pipe will block until another
* thread reads those bytes, or some previously-written bytes, from the pipe is
* system-dependent and therefore unspecified.  Implementations will
* buffer up to a certain number of bytes between the sink and source channels.
 </p>
*/
class CHANNEL_PUBLIC Pipe:private boost::noncopyable {
public:
	Pipe(const byte_buffer& buffer) throw (std::bad_alloc);
	SWriteChannel sink() BOOST_NOEXCEPT_OR_NOTHROW;
	SReadChannel source() BOOST_NOEXCEPT_OR_NOTHROW;
private:
	boost::shared_ptr<_pipe::PipeChannel> pipeCh_;
	bool volatile canRead_;
};

} // namespace io

#endif // CHANNEL_PIPE_HPP_INCLUDED
