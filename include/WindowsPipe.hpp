#ifndef CHANNEL_WINDOWS_PIPE_HPP_INCLUDED
#define CHANNEL_WINDOWS_PIPE_HPP_INCLUDED

// boost imports
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

// modules imports
#include <channels.hpp>
#include <file.hpp>

namespace io {

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
* buffer up to a certain number of bytes between the sink and source channels,
* buffering is provided by underlying operating system.
 </p>
*/
class CHANNEL_PUBLIC WindowsPipe {
private:
	typedef boost::unique_lock<boost::mutex> lock_t;
public:
	WindowsPipe() throw(io_exception);
	SWriteChannel sink() BOOST_NOEXCEPT_OR_NOTHROW;
	SReadChannel source() BOOST_NOEXCEPT_OR_NOTHROW;
	~WindowsPipe() BOOST_NOEXCEPT_OR_NOTHROW;
private:
	boost::mutex mutex_;
	boost::condition_variable_any condition_;
	HANDLE sink_;
	HANDLE source_;
	bool canRead_;
};

}// namespace io

#endif // CHANNEL_WINDOWS_PIPE_HPP_INCLUDED
