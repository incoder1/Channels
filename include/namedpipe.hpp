#ifndef __IO_NAMEDPIPE_HPP_INCLUDED__
#define __IO_NAMEDPIPE_HPP_INCLUDED__

#include "channels.hpp"

namespace io {

SReadWriteChannel CHANNEL_PUBLIC open_named_pipe(const char* pipeName);
SReadWriteChannel CHANNEL_PUBLIC create_named_pipe(const char* pipeName);

} // namesapce io

#endif // __IO_NAMEDPIPE_HPP_INCLUDED__
