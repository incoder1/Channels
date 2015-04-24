#ifndef __IO_UNIX_CONSOLE_HPP_INCLUDED__
#define __IO_UNIX_CONSOLE_HPP_INCLUDED__

#include <cstdio>
#include "channels.hpp"

namespace io {

class CHANNEL_PUBLIC ConsoleChannel:public virtual ReadWriteChannel, public virtual object {
public:
	ConsoleChannel(int fd);
	virtual std::size_t read(byte_buffer& buffer);
	virtual std::size_t write(const byte_buffer& buffer);
	virtual ~ConsoleCahannel() BOOST_NOEXCEPT_OR_NOTHROW;
private:
	int fd_;
};

class Console {
public:
	Console(bool wide) {
	}
	inline SWriteChannel out() {
		return SWriteChannel(new ConsoleCahannel(STDOUT_FILENO));
	}
	SWriteChannel err() {
		return SWriteChannel(new ConsoleCahannel(STDERR_FILENO));
	}
	SWriteChannel in() {
		return SReadChannel(new ConsoleCahannel(STDIN_FILENO));
	}
};

} // namespace io

#endif // __IO_UNIX_CONSOLE_HPP_INCLUDED__
