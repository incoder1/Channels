#ifndef __WIDOWS_NAMEDPIPE_HPP_INCLUDED__
#define __WIDOWS_NAMEDPIPE_HPP_INCLUDED__

#include "WindowsFile.hpp"

namespace io {

SReadWriteChannel CHANNEL_PUBLIC open_named_pipe(const char* pipeName);
SReadWriteChannel CHANNEL_PUBLIC create_named_pipe(const char* pipeName);

} // namespace io

#endif // __WIDOWS_NAMEDPIPE_HPP_INCLUDED__
