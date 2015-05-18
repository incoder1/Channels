#ifndef XMLSOURCE_HPP_INCLUDED
#define XMLSOURCE_HPP_INCLUDED

#include <text.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace xml {

class Source {
protected:
	Source() BOOST_NOEXCEPT_OR_NOTHROW;
public:
	virtual bool hasNext() = 0;
	virtual uint8_t nextByte()= 0;
	virtual ~Source() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

class SimpleSource:public Source {
public:
	SimpleSource(io::SReadChannel data,const io::byte_buffer& readBuf) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~SimpleSource() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual bool hasNext();
	virtual uint8_t nextByte();
protected:
	std::size_t readMore();
	inline io::byte_buffer readBuff() {
		return buff_;
	}
private:
	io::SReadChannel src_;
	io::byte_buffer buff_;
};

class ConvertingSource:public SimpleSource
{
	private:
		std::size_t readMore();
	public:
		ConvertingSource(io::SReadChannel data,io::SConverter converter,const io::byte_buffer& readBuff);
		virtual ~ConvertingSource() BOOST_NOEXCEPT_OR_NOTHROW;
		virtual bool hasNext();
		virtual uint8_t nextByte();
	private:
		io::SConverter converter_;
		io::SReadChannel src_;
		io::byte_buffer convBuff_;
};

typedef boost::shared_ptr<Source> SSource;

inline SSource make_source(io::SReadChannel data,std::size_t readBufferSize)
{
	return SSource(new SimpleSource(data,io::byte_buffer::heap_buffer(readBufferSize)));
}

inline SSource make_source(io::SReadChannel data,const io::Charset* charset, std::size_t readBufferSize)
{
	io::SConverter conv = io::make_converter(charset, io::Charsets::utf8());
	return SSource(new ConvertingSource(data,conv,io::byte_buffer::heap_buffer(readBufferSize)));
}

} // namespace xml

#endif // XMLSOURCE_HPP_INCLUDED
