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
public:
	virtual size_t read(byte_buffer& buffer) throw(io_exception) = 0;
	virtual ~ReadChannel() {}
};

typedef boost::shared_ptr<ReadChannel> PReadChannel;

class CHANNEL_PUBLIC WriteChannel {
public:
	virtual size_t write(const byte_buffer& buffer) throw(io_exception) = 0;
	virtual ~WriteChannel() {}
};

typedef boost::shared_ptr<WriteChannel> PWriteChannel;

class CHANNEL_PUBLIC ReadWriteChannel:public ReadChannel,public WriteChannel
{
public:
	ReadWriteChannel() BOOST_NOEXCEPT:
		ReadChannel(),
		WriteChannel()
	{}
	//virtual void seek(size_t count) throw(io_exception) = 0;
};

typedef  boost::shared_ptr<ReadWriteChannel> PReadWriteChannel;

} // namespace io

#endif // CHANNELS_HPP_INCLUDED
