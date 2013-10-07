#ifndef CSTAX_HPP_INCLUDED
#define CSTAX_HPP_INCLUDED

#include <charbuffers.hpp>
#include <reader.hpp>

#include <boost/unordered_set.hpp>
#include <boost/shared_ptr.hpp>

#ifndef NOEXCEPT
#	define NOEXCEPT BOOST_NOEXCEPT_OR_NOTHROW
#endif

namespace cstax {

class Attribute {
private:
	std::wstring name_;
	std::wstring value_;
public:
	Attribute(const std::wstring& name, std::wstring value) BOOST_NOEXCEPT_OR_NOTHROW:
		name_(name),value_(value)
	{}
	inline std::wstring getName() const {
		return name_;
	}
	inline void setName(const std::wstring& name) {
		name_ = name;
	}
	inline std::wstring getValue() const {
		return value_;
	}
	inline void setValue(const std::wstring& value) {
		value_ = value;
	}
};

enum XMLEvenType {
	START_ELEMENT,
	END_ELEMENT,
	CHARACTERS,
	ATTRIBUTE,
	NAMESPACE,
	PROCESSING_INSTRUCTION,
	COMMENT,
	START_DOCUMENT,
	END_DOCUMENT,
	DTD
};

class XMLEvent {
private:
	XMLEvenType type_;
protected:
	XMLEvent(const XMLEvenType& type) NOEXCEPT:
		type_(type)
	{}
public:
	XMLEvenType type() const {
		return type_;
	}
	virtual ~XMLEvent() = 0;
};

class ElementEvent {
private:
	std::wstring uri_;
	std::wstring localName_;
protected:
	ElementEvent(const std::wstring uri, const std::wstring& localName) NOEXCEPT:
		uri_(uri),
		localName_(localName)
	{}
public:
	std::wstring uri() const {
		return uri_;
	}
	std::wstring localName() const {
		return localName_;
	}
	virtual ~ElementEvent() = 0;
};

class StartElementEvent:public virtual XMLEvent,public ElementEvent {
public:
	StartElementEvent(const std::wstring& uri, const std::wstring& localName) NOEXCEPT:
		XMLEvent(START_ELEMENT),
		ElementEvent(uri, localName)
	{}
};

class EndElementEvent:public virtual XMLEvent,public ElementEvent {
public:
	EndElementEvent(const std::wstring& uri, const std::wstring& localName) NOEXCEPT:
		XMLEvent(END_ELEMENT),
		ElementEvent(uri, localName)
	{}
};

class AttributeEvent:public XMLEvent {
private:
	Attribute attr_;
public:
	AttributeEvent(const Attribute& attr) NOEXCEPT:
		XMLEvent(ATTRIBUTE),
		attr_(attr)
	{}
};

class XMLSyntax:private boost::noncopyable {
private:
	static const wchar_t WHITESPACES[26];
	static const wchar_t SYNTAX[7];
	boost::unordered_set<wchar_t> whitespaces_;
	boost::unordered_set<wchar_t> syntax_;
public:
	XMLSyntax() BOOST_NOEXCEPT_OR_NOTHROW;
	inline bool isWhiteSpace(const wchar_t charCode) {
		return whitespaces_.end() != whitespaces_.find(charCode);
	}
	inline bool isSyntaxSymbol(const wchar_t charCode) {
		return syntax_.end() != syntax_.find(charCode);
	}
};

class xml_stream_exception:virtual std::runtime_error {
public:
	xml_stream_exception(const std::string& msg) NOEXCEPT:
		std::runtime_error(msg)
	{
	}
};

class XMLSource {
public:
	typedef io::char_buffer<wchar_t> ubuff;
	typedef io::Reader<ubuff, io::conveter<wchar_t> > UTFReader;
private:
	UTFReader src_;
	XMLSource(UTFReader& source) NOEXCEPT:
		src_(source)
	{}
public:
	static XMLSource fromChannel(io::PReadChannel channel, io::charset_t charset, size_t buff) throw(io::io_exception) {
		UTFReader reader = io::create_reader(channel, io::conveter<wchar_t>(charset,io::UCS_2), (size_t)1024);
		return XMLSource(reader);
	}
	inline size_t read(ubuff& buff) throw(xml_stream_exception) {
		try {
			return src_.read(buff);
		} catch(io::io_exception& e) {
			throw xml_stream_exception("IO error");
		}
	}
};

class StreamReader {
private:
	XMLSource src_;
	XMLSyntax syntax_;
public:
	typedef boost::shared_ptr<XMLEvent> PXMLEvent;
	StreamReader(XMLSource source) NOEXCEPT:
		src_(source)
	{}
	PXMLEvent next() throw(xml_stream_exception);
	PXMLEvent nextTag() throw(xml_stream_exception);
	bool hasNext() throw(xml_stream_exception);
	std::wstring getElementText() throw(xml_stream_exception);
};

} // namesapace cstax

#endif // CSTAX_HPP_INCLUDED
