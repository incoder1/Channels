#ifndef READER_HPP_INCLUDED
#define READER_HPP_INCLUDED

#include <stdexcept>
#include <convert.hpp>
#include <channels.hpp>

namespace io {

/// Used when no character set conversation needed
class EmptyConverter:public Converter {
public:
	EmptyConverter(const Charset* ch) BOOST_NOEXCEPT_OR_NOTHROW:
		Converter(ch,ch)
	{}
	virtual ~EmptyConverter() BOOST_NOEXCEPT_OR_NOTHROW
	{}
	virtual void convert(const byte_buffer& src, byte_buffer& dest) throw(charset_exception) {
		dest = src;
		dest.move(src.length());
	}
};

namespace _private {
	template<class T>
	class no_free_static_ref {
		public:
			inline void operator()(T*) {
			}
	};
}

inline SConverter char_empty_converter() {
	static Charset emptyCharset(sizeof(char),"CHAR",sizeof(char),false);
	static EmptyConverter converter(&emptyCharset);
	static _private::no_free_static_ref<EmptyConverter> noFree;
	return SConverter(const_cast<EmptyConverter*>(&converter), noFree);
}

inline SConverter wchar_t_empty_converter() {
	static Charset emptyCharset(sizeof(wchar_t),"WCHAR_T",sizeof(wchar_t),true);
	static EmptyConverter converter(&emptyCharset);
	static _private::no_free_static_ref<EmptyConverter> noFree;
	return SConverter(const_cast<EmptyConverter*>(&converter), noFree);
}

/**
 * ! \brief Reader template, provides functionality for read STL strings from read channel,
 *  with automatic character set conversation
 * \param String a type of STL string i.e. std::basic_string<char8_t>, std::string, std::wstring etc.
 */
template<class String>
class Reader {
private:
	typedef typename String::value_type _TChar;
public:
	/**
	 * Constructs new reader with character set converting.
	 * Note, use an empty converter in case when conversation is not needed
	 * \param src source channel
	 * \param buff internal read buffer
	 * \param conv character set converter.
	 */
	Reader(SReadChannel src, const byte_buffer& buff,SConverter conv) BOOST_NOEXCEPT_OR_NOTHROW:
		src_(src),
		buff_(buff),
		conv_(conv)
	{}
	String read() throw(io_exception,charset_exception) {
		buff_.clear();
		std::size_t bytesRead = src_->read(buff_);
		const std::size_t maxChSize = conv_->destCharset()->charSize();
		byte_buffer conv = byte_buffer::heap_buffer(bytesRead*maxChSize);
		buff_.flip();
		conv_->convert(buff_,conv);
		return String(conv.position(),conv.last());
	}
private:
	SReadChannel src_;
	byte_buffer buff_;
	SConverter conv_;
};

/**
 * ! \brief Writer temple, provides functionality for writing strings into write channel.
 * Strings can be converted from one character set into another
 */
template<class String>
class Writer {
private:
	typedef typename String::value_type _TChar;
public:
	/**
	 * Constructs new writer to write converted string.
	 * Note, use an empty converter in case when conversation is not needed
	 * \param out smart pointer to the write channel
	 * \param conv smart pointer ti the character converter
	 */
	Writer(SWriteChannel out,SConverter conv) BOOST_NOEXCEPT_OR_NOTHROW:
		out_(out),
		conv_(conv)
	{}

	/**
	 * Writes STL string into write channel
	 */
	void write(const String& str) throw(io_exception,charset_exception) {
		byte_buffer buff = byte_buffer::wrap_array(str.data(), str.length()); // 0 ending symbol should be avoided
		write(buff);
	}

	void write(const char* str) throw (io_exception, charset_exception) {
		byte_buffer buff = byte_buffer::wrap_str(str);
		write(buff);
	}

	void write(const byte_buffer& buff) throw(io_exception, charset_exception, std::bad_alloc) {
		const std::size_t destCharSize = conv_->destCharset()->charSize();
		byte_buffer convBytes = byte_buffer::heap_buffer(buff.length()*destCharSize);
		conv_->convert(buff, convBytes);
		convBytes.flip();
		out_->write(convBytes);
	}
private:
	SWriteChannel out_;
	SConverter conv_;
};

} // namespace io

#endif // READER_HPP_INCLUDED
