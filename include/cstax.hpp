#ifndef CSTAX_HPP_INCLUDED
#define CSTAX_HPP_INCLUDED

#include <readwrite.hpp>

#include <stack>

#include <boost/unordered_set.hpp>
#include <boost/shared_ptr.hpp>

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
	XMLEvent(const XMLEvenType& type) BOOST_NOEXCEPT_OR_NOTHROW:
		type_(type)
	{}
public:
	XMLEvenType type() const {
		return type_;
	}
	virtual ~XMLEvent() = 0;
};

typedef boost::shared_ptr<XMLEvent> PXMLEvent;

class ElementEvent {
private:
	std::wstring uri_;
	std::wstring localName_;
protected:
	ElementEvent(const std::wstring uri, const std::wstring& localName) BOOST_NOEXCEPT_OR_NOTHROW:
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
	StartElementEvent(const std::wstring& uri, const std::wstring& localName) BOOST_NOEXCEPT_OR_NOTHROW:
		XMLEvent(START_ELEMENT),
		ElementEvent(uri, localName)
	{}
};

class EndElementEvent:public virtual XMLEvent,public ElementEvent {
public:
	EndElementEvent(const std::wstring& uri, const std::wstring& localName) BOOST_NOEXCEPT_OR_NOTHROW:
		XMLEvent(END_ELEMENT),
		ElementEvent(uri, localName)
	{}
};

class AttributeEvent:public XMLEvent {
private:
	Attribute attr_;
public:
	AttributeEvent(const Attribute& attr) BOOST_NOEXCEPT_OR_NOTHROW:
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
	xml_stream_exception(const std::string& msg) BOOST_NOEXCEPT_OR_NOTHROW:
		std::runtime_error(msg)
	{}
};

class XMLSource {
public:
	typedef io::char_buffer<wchar_t> ubuff;
	//typedef io::Reader< io::conveter<wchar_t> > UTFReader;
private:
//	UTFReader src_;
//	XMLSource(UTFReader& source) BOOST_NOEXCEPT_OR_NOTHROW:
//		src_(source)
//	{}
public:
	inline size_t read(ubuff& buff) throw(xml_stream_exception) {
//		try {
//			return src_.read(buff);
//		} catch(io::io_exception& e) {
//			throw xml_stream_exception("IO error");
//		}
	return 0;
	}
};

class StreamReader {
private:
	typedef XMLSource::ubuff ubuff;
	XMLSource src_;
	const size_t buffSize_;
	XMLSyntax syntax_;
public:
	typedef boost::shared_ptr<XMLEvent> PXMLEvent;
	StreamReader(const XMLSource& source,size_t buffSize) BOOST_NOEXCEPT_OR_NOTHROW;
	PXMLEvent next() throw(xml_stream_exception);
	PXMLEvent nextTag() throw(xml_stream_exception);
	bool hasNext() throw(xml_stream_exception);
	std::wstring getElementText() throw(xml_stream_exception);
};

} // namesapace cstax

#endif // CSTAX_HPP_INCLUDED
