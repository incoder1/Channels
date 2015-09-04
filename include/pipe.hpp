#ifndef CHANNEL_PIPE_HPP_INCLUDED
#define CHANNEL_PIPE_HPP_INCLUDED

// boost imports
#include <boost/thread/thread.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

// modules imports
#include "channels.hpp"

namespace io {

/**
 * ! \brief a functional object (functor) type to be used by client to write some data into pipe
 */
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
	/**
	 * Call the client write routine, implementor must create and start the thread for this propose
	 * \param channel an initialized sink write channel
	 */
	void call_sink_routine(const SWriteChannel& channel) const;
public:
	/**
	 * Returns reference on source read channel
	 */
	virtual SReadChannel source() const = 0;
	/**
	 * Destroy pipe and closes all associates system resources
	 * \throw never throw
	 */
	virtual ~Pipe() BOOST_NOEXCEPT_OR_NOTHROW;
private:
	PipeSinkRoutine sinkRoutine_;
};

DECLARE_PTR_T(Pipe);

/**
 * Creates and initialize the Pipe. Sink functor will be used as the routine for the new thread
 * \param buffer_size - the RAM buffer size in bytes
 * \param sink, a functional object which is used for writing data to the sink write channel
 * \return smart reference on Pipe
 */
SPipe CHANNEL_PUBLIC create_pipe(std::size_t buffer_size, PipeSinkRoutine sink);


} // namespace io

#endif // CHANNEL_PIPE_HPP_INCLUDED
