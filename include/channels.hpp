#ifndef CHANNELS_HPP_INCLUDED
#define CHANNELS_HPP_INCLUDED

#include <stdexcept>
#include <boost/shared_ptr.hpp>

#include <channels_config.h>
#include <bytebuffer.hpp>

namespace io {

class io_exception: public std::runtime_error {
public:
	explicit io_exception(const std::string& what):
		std::runtime_error(what)
	{}
};


class CHANNEL_PUBLIC ReadChannel {
#ifdef BOOST_NO_DELETED_FUNCTIONS
private:  // emphasize the following members are private
	ReadChannel( const ReadChannel&);
	ReadChannel& operator=( const ReadChannel& );
#else
	ReadChannel( const ReadChannel& ) = delete;
	ReadChannel& operator=( const ReadChannel& ) = delete;
#endif
protected:
	ReadChannel() BOOST_NOEXCEPT_OR_NOTHROW;
public:
	virtual std::size_t read(byte_buffer& buffer) throw(io_exception) = 0;
	virtual ~ReadChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

typedef boost::shared_ptr<ReadChannel> SReadChannel;

class CHANNEL_PUBLIC WriteChannel {
#ifdef BOOST_NO_DELETED_FUNCTIONS
private:  // emphasize the following members are private
	WriteChannel( const WriteChannel&);
	WriteChannel& operator=( const WriteChannel& );
#else
	WriteChannel( const WriteChannel& ) = delete;
	WriteChannel& operator=( const WriteChannel& ) = delete;
#endif
protected:
	WriteChannel() BOOST_NOEXCEPT_OR_NOTHROW;
public:
	virtual std::size_t write(const byte_buffer& buffer) throw(io_exception) = 0;
	virtual ~WriteChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

typedef boost::shared_ptr<WriteChannel> SWriteChannel;

typedef enum {FROM_BEGIN = 0 , FROM_CURRENT_POSITION = 1, FROM_END = 2} MoveMethod;

class CHANNEL_PUBLIC ReadWriteChannel:public virtual ReadChannel,public virtual WriteChannel {
#ifdef BOOST_NO_DELETED_FUNCTIONS
private:  // emphasize the following members are private
	ReadWriteChannel( const ReadChannel&);
	ReadWriteChannel& operator=( const ReadChannel& );
#else
	ReadWriteChannel( const ReadWriteChannel& ) = delete;
	ReadWriteChannel& operator=( const ReadWriteChannel& ) = delete;
#endif
protected:
	ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW;
public:
	/**
	 * Moves read/write pointer
	 * \param offset offset in bytes
	 * \param method moving method
	 */
	virtual void seek(std::size_t offset, MoveMethod method) throw(io_exception) = 0;
	virtual ~ReadWriteChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

typedef boost::shared_ptr<ReadWriteChannel> SReadWriteChannel;

class CHANNEL_PUBLIC AsynchronousCompletionHandler {
#ifdef BOOST_NO_DELETED_FUNCTIONS
private:
	AsynchronousCompletionHandler(const AsynchronousCompletionHandler&);
	AsynchronousCompletionHandler& operator=(const AsynchronousCompletionHandler& );
#else
	AsynchronousCompletionHandler(const AsynchronousCompletionHandler&) = delete;
	AsynchronousCompletionHandler& operator=(const AsynchronousCompletionHandler& ) = delete;
#endif
protected:
	AsynchronousCompletionHandler() BOOST_NOEXCEPT_OR_NOTHROW;
public:
	virtual void success(const byte_buffer&,std::size_t size) = 0;
	virtual void failed(const char* errorMessage) = 0;
	virtual ~AsynchronousCompletionHandler() BOOST_NOEXCEPT_OR_NOTHROW;
};

typedef boost::shared_ptr<AsynchronousCompletionHandler> SAsynchCompletionHandler;

class CHANNEL_PUBLIC AsynchronousReadChannel {
#ifdef BOOST_NO_DELETED_FUNCTIONS
private:
	AsynchronousReadChannel(const AsynchronousReadChannel&);
	AsynchronousReadChannel& operator=(const AsynchronousReadChannel&);
#else
	AsynchronousReadChannel(const AsynchronousReadChannel&) = delete;
	AsynchronousReadChannel& operator=(const AsynchronousReadChannel&) = delete;
#endif // BOOST_NO_DELETED_FUNCTIONS
protected:
	AsynchronousReadChannel() BOOST_NOEXCEPT_OR_NOTHROW;
public:
	virtual ~AsynchronousReadChannel() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual void read(const byte_buffer& buffer, SAsynchCompletionHandler handler) throw(io_exception) = 0;
};

typedef boost::shared_ptr<AsynchronousReadChannel> SAsynchReadChannel;

} // namespace io

#endif // CHANNELS_HPP_INCLUDED
