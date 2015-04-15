#ifndef __IO_NAMEDPIPE_HPP_INCLUDED__
#define __IO_NAMEDPIPE_HPP_INCLUDED__

#include <boost/function.hpp>
#include "channels.hpp"

namespace io {


typedef boost::function<void(std::size_t read,const byte_buffer&)> client_connection_handler;

class CHANNEL_PUBLIC PipeServer
{
BOOST_MOVABLE_BUT_NOT_COPYABLE(PipeServer)
protected:
	PipeServer() BOOST_NOEXCEPT_OR_NOTHROW;
public:
	virtual ~PipeServer() BOOST_NOEXCEPT_OR_NOTHROW = 0;
	virtual void start() = 0;
};

typedef boost::shared_ptr<PipeServer> SPipeServer;

//SReadWriteChannel CHANNEL_PUBLIC open_named_pipe(const char* pipeName);
//SReadWriteChannel CHANNEL_PUBLIC create_named_pipe(const char* pipeName);

} // namesapce io

#endif // __IO_NAMEDPIPE_HPP_INCLUDED__
