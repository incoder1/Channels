#ifndef CHANNEL_PIPE_HPP_INCLUDED
#define CHANNEL_PIPE_HPP_INCLUDED

// boost imports
#include <boost/thread/thread.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

// modules imports
#include <channels.hpp>

namespace io {


typedef boost::function<void(SWriteChannel)> PipeSinkRoutine;

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
class CHANNEL_PUBLIC Pipe:public object {
protected:
	Pipe(PipeSinkRoutine routine) BOOST_NOEXCEPT_OR_NOTHROW;
	void call_sink_routine(const SWriteChannel& channel) const;
public:
	virtual SReadChannel source() const = 0;
	virtual ~Pipe() BOOST_NOEXCEPT_OR_NOTHROW;
private:
	PipeSinkRoutine sinkRoutine_;
};

typedef boost::shared_ptr<Pipe> SPipe;

SPipe CHANNEL_PUBLIC create_pipe(std::size_t buffer_size, PipeSinkRoutine sink) ;

} // namespace io

#endif // CHANNEL_PIPE_HPP_INCLUDED
