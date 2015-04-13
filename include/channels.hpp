#ifndef __CHANNELS_HPP_INCLUDED__
#define __CHANNELS_HPP_INCLUDED__

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/move/move.hpp>

#include "errors.hpp"
#include "bytebuffer.hpp"

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
	ReadChannel() BOOST_NOEXCEPT_OR_NOTHROW;
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
typedef boost::shared_ptr<ReadChannel> SReadChannel;

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
	WriteChannel() BOOST_NOEXCEPT_OR_NOTHROW;
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
typedef boost::shared_ptr<WriteChannel> SWriteChannel;

/**
 * ! \brief Channel which allows reading of and writing into the same resource
 * Implementor must build on source which allows reading and writing at the same time,
 * like file or named pipe for example
 */
class ReadWriteChannel:public virtual ReadChannel,public virtual WriteChannel
{
BOOST_MOVABLE_BUT_NOT_COPYABLE(ReadWriteChannel)
protected:
	ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW;
public:
	virtual std::size_t read(byte_buffer& buffer) = 0;
	virtual std::size_t write(const byte_buffer& buffer) = 0;
	virtual ~ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

typedef boost::shared_ptr<ReadWriteChannel> SReadWriteChannel;

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
	RandomAccessChannel() BOOST_NOEXCEPT_OR_NOTHROW;
public:
	/**
	* Return current channel position
	* \return underlying resource current position
	*/
	virtual std::size_t position() = 0;
	/**
	 * Moves current channel position forward from current on offset bytes
	 * \throw io_exception in case of system error
	 * \return new position channel position
	 */
	virtual std::size_t forward(std::size_t offset) = 0;
	/**
	 * Moves current channel position backward from current on offset bytes
	 * \throw io_exception in case of system error
	 * \return new position channel position
	 */
	virtual std::size_t backward(std::size_t offset) = 0;
	/**
	 * Moves current channel position forward from channel begin on offset bytes
	 * \throw io_exception in case of system error
	 * \return new position channel position
	 */
	virtual std::size_t fromBegin(std::size_t offset) = 0;
	/**
	 * Moves current channel position backward from channel end on offset bytes
	 * \throw io_exception in case of system error
	 * \return new position channel position
	 */
	virtual std::size_t fromEnd(std::size_t offset)  = 0;

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
typedef boost::shared_ptr<RandomAccessChannel> SRandomAccessChannel;

/**
* ! \brief Generic abstract class for asynchronous reading from an underlying resource
*/
class CHANNEL_PUBLIC AsynchReadChannel {
public:
	/**
	 * Functor for handling state of asynchronous read data available
	 */
	typedef boost::function<void(boost::system::error_code&,std::size_t,const byte_buffer&)> read_callback;
	/**
	 * Initialize this channel with read data available handler, to be called by implementor
	 */
	explicit AsynchReadChannel(const read_callback& callback) BOOST_NOEXCEPT_OR_NOTHROW;
	/**
	 * Pure virtual destructor, never throws
	 */
	virtual ~AsynchReadChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
protected:
	/**
	 * Calls handler functor, to be used by implementor
	 * \param err
	 *				an error which describing reading state. Implementor must also transfer
	 *				success or EOF states
	 * \param read
	 *				bytes read count
	 * \param data
	 * 				read data as a memory buffer
	 */
	void handleRead(boost::system::error_code& err, std::size_t read,const byte_buffer& data) const;
private:
	read_callback callback_;
};


/**
* Transfers data from source read channel to the destination write channel using memory buffer
* \param src source channel
* \param dst destination buffer
* \param buffer memory block buffer for copying data
*/
inline void transfer(const SReadChannel& src,const SWriteChannel& dst, byte_buffer& buffer)
{
	while(src->read(buffer) > 0) {
		buffer.flip();
		dst->write(buffer);
		buffer.clear();
	}
}

} // namespace io

#endif // __CHANNELS_HPP_INCLUDED__
