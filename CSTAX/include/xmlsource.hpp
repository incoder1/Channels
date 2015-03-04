#ifndef XMLSOURCE_HPP_INCLUDED
#define XMLSOURCE_HPP_INCLUDED

#include <text.hpp>

namespace xml {


class Source
{
	public:
		static Source create(io::SReadChannel source);
		static Source create(io::SReadChannel src, const char* inputChaset);
		bool hasNext();
		uint8_t nextByte();
	private:
		Source(io::SReadChannel source,io::SConverter converter,const io::byte_buffer& readBuf,const io::byte_buffer& convBuff) BOOST_NOEXCEPT_OR_NOTHROW;
		std::size_t readMore();
		std::size_t convertMore();
	private:
		io::SConverter converter_;
		io::SReadChannel src_;
		io::byte_buffer readBuff_;
		io::byte_buffer convBuff_;
		io::SWriteChannel dbg;
};


} // namespace xml

#endif // XMLSOURCE_HPP_INCLUDED
