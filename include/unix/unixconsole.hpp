#ifndef __IO_UNIX_CONSOLE_HPP_INCLUDED__
#define __IO_UNIX_CONSOLE_HPP_INCLUDED__

#include <langinfo.h>
#include <locale.h>
#include <cstdlib>

#include "channels.hpp"
#include "charsets.hpp"

namespace io {

class ConsoleChannel:public ReadWriteChannel {
public:
    explicit ConsoleChannel(int fd);
    virtual ~ConsoleChannel() BOOST_NOEXCEPT_OR_NOTHROW;
    virtual std::size_t read(byte_buffer& buffer);
    virtual std::size_t write(const byte_buffer& buffer);
private:
    int fd_;
};

class Console {
public:
    Console(bool wide):
        charset_(determSysCharset())
    {}

    inline SWriteChannel out()
    {
        return SWriteChannel(new ConsoleChannel(STDOUT_FILENO));
    }
    SWriteChannel err()
    {
        return SWriteChannel(new ConsoleChannel(STDERR_FILENO));
    }
    SReadChannel in()
    {
        return SReadChannel(new ConsoleChannel(STDIN_FILENO));
    }
    inline const Charset* charset() {
		return charset_; // shell be UTF8 in all cases
	}
	void setCharset(const Charset* charset) {
        // Operation is not supported, since always UTF8 unicode
	}
private:
    inline const Charset* determSysCharset() {
        char* charsetName = ::nl_langinfo(CODESET);
        const Charset* result = Charsets::forName(charsetName);
        return result;
    }
private:
    const Charset* charset_;
};

} // namespace io

#endif // __IO_UNIX_CONSOLE_HPP_INCLUDED__
