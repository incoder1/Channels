#ifndef CHANNELS_NETWORK_HPP_INCLUDED
#define CHANNELS_NETWORK_HPP_INCLUDED

#include <smallobjectpool.hpp>
#include <channels.hpp>

#ifdef __MINGW32__
#	include <winsock2.h>
extern "C" BOOL WINAPI UnregisterWaitEx(HANDLE,HANDLE);
#endif // Mingw32

#include <boost/asio.hpp>

namespace io {

namespace net {

} // namespace net

} // namespace io
#endif // CHANNELS_NETWORK_HPP_INCLUDED
