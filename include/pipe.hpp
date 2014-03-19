#ifndef CHANNEL_PIPE_HPP_INCLUDED
#define CHANNEL_PIPE_HPP_INCLUDED

// boost imports
#include <boost/thread.hpp>
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
class CHANNEL_PUBLIC Pipe {
protected:
	/// Only for implementor
	explicit Pipe() BOOST_NOEXCEPT_OR_NOTHROW;
public:
	virtual ~Pipe() BOOST_NOEXCEPT_OR_NOTHROW = 0;
	/**
	 * Opens the sink writable channel
	 * \return smart pointer to the sink channel
	 */
	virtual SWriteChannel sink() BOOST_NOEXCEPT_OR_NOTHROW = 0;
	/**
	 * Opens the source readable channel
	 * \return smart pointer to the source channel
	 */
	virtual SReadChannel source() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

typedef boost::shared_ptr<Pipe> SPipe;

/**
 * Constructs new pipe for the current operating system
 * \return smart pointer on pipe
 */
SPipe CHANNEL_PUBLIC open_pipe() throw(io_exception);

#ifdef PLATFROM_WINDOWS
class WindowsPipe:public Pipe {
private:
	typedef boost::unique_lock<boost::mutex> lock_t;
public:
	WindowsPipe(HANDLE sink, HANDLE source) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual SWriteChannel sink() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual SReadChannel source() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~WindowsPipe() BOOST_NOEXCEPT_OR_NOTHROW;
private:
	boost::mutex mutex_;
	boost::condition_variable_any condition_;
	bool canRead_;
	HANDLE sink_;
	HANDLE source_;
};
#endif

} // namespace io

#endif // CHANNEL_PIPE_HPP_INCLUDED
