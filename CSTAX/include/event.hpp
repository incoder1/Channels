#ifndef EVNET_HPP_INCLUDED
#define EVNET_HPP_INCLUDED

// boost
#include <boost/shared_ptr.hpp>
// io
#include <smallobject.hpp>
// self
#include "element.hpp"

namespace xml {

enum EvenType {
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

class Event;
typedef boost::shared_ptr<Event> SEvent;

class Event:public virtual io::object,public virtual boost::enable_shared_from_this<Event> {
protected:
	explicit Event(EvenType type) BOOST_NOEXCEPT_OR_NOTHROW;
public:
	EvenType type() const {
		return type_;
	}
private:
	EvenType type_;
};

template<class detail_ptr>
inline detail_ptr event_up_cast(SEvent ev)
{
	typedef typename detail_ptr::element_type UpType;
	return boost::dynamic_pointer_cast<UpType,Event>(ev->shared_from_this());
};

class DocumentEvent:public Event {
public:
	DocumentEvent(const std::string& version, const std::string& encoding, bool standalone) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~DocumentEvent() BOOST_NOEXCEPT_OR_NOTHROW;
	std::string version() const {
		return std::string(version_);
	}
	std::string encoding() const {
		return std::string(encoding_);
	}
	bool isStandalone() const {
		return standalone_;
	}
private:
	std::string version_;
	std::string encoding_;
	bool standalone_;
};

typedef boost::shared_ptr<DocumentEvent> SDocumentEvent;

class ElementEvent:public Event {
public:
	typedef boost::shared_ptr<ElementEvent> reference;
	ElementEvent(EvenType type, const std::string& uri, const std::string& localName) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~ElementEvent() BOOST_NOEXCEPT_OR_NOTHROW;
	inline std::string uri() const {
		return std::string(uri_);
	}
	inline std::string localName() const {
		return std::string(localName_);
	}
private:
	const std::string uri_;
	const std::string localName_;
};

typedef boost::shared_ptr<ElementEvent> SElementEvent;

//class AttributeEvent:public Event {
//public:
//	AttributeEvent(const Attribute& attr) BOOST_NOEXCEPT_OR_NOTHROW:
//		Event(ATTRIBUTE),
//		attr_(attr)
//	{}
//	inline Attribute getAttribute() {
//		return attr_;
//	}
//private:
//	Attribute attr_;
//};

//class CharactersEvent:public Event {
//public:
//	CharactersEvent() BOOST_NOEXCEPT_OR_NOTHROW:
//		Event(CHARACTERS)
//	{}
//};

} // xmlevent

#endif // EVNET_HPP_INCLUDED
