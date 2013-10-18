#ifndef CHANNELS_HPP_INCLUDED
#define CHANNELS_HPP_INCLUDED

#include <stdexcept>
#include <boost/shared_ptr.hpp>

#include <channels_config.h>
#include <bytebuffer.hpp>

namespace io {

class io_exception: virtual std::runtime_error {
private:
	const char* what_;
public:
	explicit io_exception(const char* what):
		std::runtime_error(what)
	{}
};

class CHANNEL_PUBLIC ReadChannel {
private:
	ReadChannel(const ReadChannel&);
protected:
	ReadChannel()
	{}
public:
	virtual size_t read(byte_buffer& buffer) throw(io_exception) = 0;
	virtual ~ReadChannel()
	{}
};

typedef boost::shared_ptr<ReadChannel> PReadChannel;

class CHANNEL_PUBLIC WriteChannel {
private:
	WriteChannel(const WriteChannel&);
protected:
	WriteChannel()
	{}
public:
	virtual size_t write(const byte_buffer& buffer) throw(io_exception) = 0;
	virtual ~WriteChannel()
	{}
};

typedef boost::shared_ptr<WriteChannel> PWriteChannel;

class CHANNEL_PUBLIC ReadWriteChannel:public ReadChannel,public WriteChannel {
public:
	enum MoveMethod {FROM_BEGIN = 0 , FROM_CURRENT_POSITION = 1, FROM_END = 2};
private:
	ReadWriteChannel(const ReadWriteChannel&);
protected:
ReadWriteChannel() BOOST_NOEXCEPT:
	ReadChannel(),
	            WriteChannel()
	{}
public:
	/**
	 * Moves read/write pointer
	 * \param offset offset in bytes
	 * \param method for moving
	 */
	virtual void seek(size_t offset, MoveMethod method) throw(io_exception) = 0;
	virtual ~ReadWriteChannel()
	{}
};

typedef  boost::shared_ptr<ReadWriteChannel> PReadWriteChannel;

} // namespace io

#endif // CHANNELS_HPP_INCLUDED
