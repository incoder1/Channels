#ifndef __CHANNELS_HPP_INCLUDED__
#define __CHANNELS_HPP_INCLUDED__

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/move/move.hpp>

#include "errors.hpp"
#include "bytebuffer.hpp"

#ifndef DECLARE_PTR_T
#	define DECLARE_PTR_T(TYPE) typedef boost::shared_ptr<TYPE> S##TYPE
#endif // DECLARE_SPTR_T

namespace io {

/**
 * ! \brief Channel which allow reading raw bytes from a source
 */
class CHANNEL_PUBLIC ReadChannel {
	BOOST_MOVABLE_BUT_NOT_COPYABLE(ReadChannel)
protected:
	/**
	 * Default constructor, to be called by implementor
	 * Never trows
	 */
	ReadChannel();
public:
	/**
	 * Reads data into byte buffer starting on current buffer position,
	 *	maximum bytes read is limited by buffer available.
	 * \param buffer - a byte buffer to hold the result
	 * \throw io_exception in case of underlying system error
	 * \return count of bytes read
	 */
	virtual std::size_t read(byte_buffer& buffer) = 0;
	/**
	 * Pure virtual interface destructor, never throws
	 */
	virtual ~ReadChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

/**
 * ! \brief Smart pointer type on ReadChannel.
 *	All channel interfaces consider to prefer the smart pointers whether
 *	the raw pointers on channel interfaces.
 */
DECLARE_PTR_T(ReadChannel);

/**
 * Channel with allow writing raw bytes into a source
 */
class CHANNEL_PUBLIC WriteChannel {
	BOOST_MOVABLE_BUT_NOT_COPYABLE(WriteChannel)
protected:
	/**
	* Default constructor to be called by implementor
	* Never throws
	*/
	WriteChannel();
public:
	/**
	 * Writes buffer date starting from current buffer position, ending current buffer limit.
	 * Source buffer moves on writing bytes count offset
	 * \param buffer data to write
	 * \throw io_exception in case of system IO error
	 * \return count of bytes written
	 */
	virtual std::size_t write(const byte_buffer& buffer) = 0;
	/**
	 * Pure virtual destructor, never throws
	*/
	virtual ~WriteChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

/**
 * ! \brief Smart pointer type on WriteChannel.
 *	All channel interfaces consider to prefer the smart pointers whether
 *	the raw pointers on channel interfaces.
 */
DECLARE_PTR_T(WriteChannel);

/**
 * ! \brief Channel which allows reading of and writing into the same resource
 * Implementor must build on source which allows reading and writing at the same time,
 * like file or named pipe for example
 */
class ReadWriteChannel:public virtual ReadChannel,public virtual WriteChannel {
	BOOST_MOVABLE_BUT_NOT_COPYABLE(ReadWriteChannel)
protected:
	ReadWriteChannel();
public:
	virtual std::size_t read(byte_buffer& buffer) = 0;
	virtual std::size_t write(const byte_buffer& buffer) = 0;
	virtual ~ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

DECLARE_PTR_T(ReadWriteChannel);


/**
* ! \brief Random access channel with: read, write and move current position operations.
* Implementor must build on top of source which allows provided operations.
* E.g. file or memory buffer is good chose
*/
class CHANNEL_PUBLIC RandomAccessChannel:public ReadWriteChannel {
	BOOST_MOVABLE_BUT_NOT_COPYABLE(RandomAccessChannel)
protected:
	/**
	* Default constructor to be called by implementor
	* Never throws
	*/
	RandomAccessChannel();
public:
	/**
	* Returns current channel position
	* \param pos
	* \return underlying resource current position
	*/
	virtual uint64_t position() = 0;
	/**
	 * Moves current channel position forward from current on offset bytes
	 * \throw io_exception in case of system error
	 */
	virtual uint64_t forward(uint64_t offset) = 0;
	/**
	 * Moves current channel position backward from current on offset bytes
	 * \throw io_exception in case of system error
	 */
	virtual uint64_t backward(uint64_t offset) = 0;
	/**
	 * Moves current channel position forward from channel begin on offset bytes
	 * \throw io_exception in case of system error
	 * \return new position channel position
	 */
	virtual uint64_t fromBegin(uint64_t offset) = 0;
	/**
	 * Moves current channel position backward from channel end on offset bytes
	 * \throw io_exception in case of system error
	 * \return new position channel position
	 */
	virtual uint64_t fromEnd(uint64_t offset)  = 0;

	virtual std::size_t read(byte_buffer& buffer) = 0;
	virtual std::size_t write(const byte_buffer& buffer) = 0;

	/**
	 * Pure virtual interface destructor, never throws
	 */
	virtual ~RandomAccessChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

/**
 * ! \brief Smart pointer type on ReadWriteChannel.
 *	All channel interfaces consider to prefer the smart pointers whether
 *	the raw pointers on channel interfaces.
 */
DECLARE_PTR_T(RandomAccessChannel);

/**
* Functor type for handling state of asynchronous data
*/
typedef boost::function<void(long,std::size_t,const byte_buffer&)> completion_routine_f;

/**
* ! \brief Asynchronous write operation AsyncResult, allows caller block current execution
* thread until operation complete or cancel operation
*/
class AsyncResult {
BOOST_MOVABLE_BUT_NOT_COPYABLE(AsyncResult)
protected:
	AsyncResult();
public:
	/**
	 * Default pure virtual descriptor
	 * \throw never throws
	 */
	virtual ~AsyncResult() BOOST_NOEXCEPT_OR_NOTHROW = 0;
	/**
	 * Blocks current execution thread until asynchronous IO operation complete
	 * \return number of bytes transfered
	 */
	virtual std::size_t await() = 0;
	/**
	 * Cancels current asynchronous IO operation
	 * \return \code true whether write operation has been canceled, \code false if operation was complete or canceled or failed
	 */
	virtual bool cancel() = 0;

	/**
	 * Checks whether asynchronous IO operation is in progress, i.e. pending state
	 * \return whether asynchronous IO operation in progress
	 */
	virtual bool active() = 0;

	/**
	 * Returns asynchronous IO operation data buffer
	 * In case of read operation,buffer contains data which were asynchronously read
	 * In case of write operation, implementor returns original buffer, moved to transfered bytes position
	 * \return data buffer
	 */
	virtual byte_buffer buffer() = 0;
};

DECLARE_PTR_T(AsyncResult);

class CHANNEL_PUBLIC AsynchReadChannel {
	BOOST_MOVABLE_BUT_NOT_COPYABLE(AsynchReadChannel)
protected:
	AsynchReadChannel();
public:
	virtual SAsyncResult read(uint64_t pos,std::size_t maxbytes) = 0;
	virtual ~AsynchReadChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

DECLARE_PTR_T(AsynchReadChannel);

class CHANNEL_PUBLIC AsynchWriteChannel {
	BOOST_MOVABLE_BUT_NOT_COPYABLE(AsynchWriteChannel)
protected:
	AsynchWriteChannel();
public:
	virtual SAsyncResult write(const byte_buffer& buffer) = 0;
	virtual ~AsynchWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

DECLARE_PTR_T(AsynchWriteChannel);

/**
* ! \brief Generic abstract class for asynchronous read/write operations from an underlying resource
*/
class CHANNEL_PUBLIC AsynchReadWriteChannel:public virtual AsynchReadChannel, public virtual AsynchWriteChannel {
	BOOST_MOVABLE_BUT_NOT_COPYABLE(AsynchReadWriteChannel)
protected:
	AsynchReadWriteChannel();
public:
	virtual SAsyncResult read(uint64_t pos,std::size_t maxbytes) = 0;
	virtual SAsyncResult write(const byte_buffer& buffer) = 0;
	virtual ~AsynchReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

DECLARE_PTR_T(AsynchReadWriteChannel);

/**
* Transfers data from source read channel to the destination write channel using a memory buffer
* \param src source channel
* \param dst destination channel
* \param buffer memory block buffer for copying data
*/
inline void transfer(const SReadChannel& src,const SWriteChannel& dst, byte_buffer& buffer)
{
	while(src->read(buffer) > 0) {
		buffer.flip();
		while(!buffer.empty()) {
			buffer.move(dst->write(buffer));
		}
		buffer.clear();
	}
}

} // namespace io

#endif // __CHANNELS_HPP_INCLUDED__
