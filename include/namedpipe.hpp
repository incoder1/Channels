#ifndef __NAMEDPIPE_HPP_INCLUDED__
#define __NAMEDPIPE_HPP_INCLUDED__

#include "channels.hpp"

namespace io {

	namespace ipc {

		SReadWriteChannel CHANNEL_PUBLIC create_duplex_named_pipe(const char* name,std::size_t bufferSize);
		SReadWriteChannel CHANNEL_PUBLIC connect_duplex_named_pipe(const char* name, byte_buffer& buffer);

	} // namespace ipc

} // namesapce io

#endif // __NAMEDPIPE_HPP_INCLUDED__
