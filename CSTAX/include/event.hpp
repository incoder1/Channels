#ifndef EVNET_HPP_INCLUDED
#define EVNET_HPP_INCLUDED

#include <smallobject.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <element.hpp>

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

class Event:public virtual boost::enable_shared_from_this<Event>, public virtual io::SmallObject {
BOOST_MOVABLE_BUT_NOT_COPYABLE(Event)
protected:
	explicit Event(EvenType type) BOOST_NOEXCEPT_OR_NOTHROW:
		boost::enable_shared_from_this<Event>(),
		io::SmallObject(),
		type_(type)
	{}
public:
	EvenType type() const {
		return type_;
	}
private:
	EvenType type_;
};

typedef boost::shared_ptr<Event> SEvent;

class DocumentEvent:public Event {
BOOST_MOVABLE_BUT_NOT_COPYABLE(DocumentEvent)
public:
	DocumentEvent(const std::string& version, const std::string& encoding, bool standalone) BOOST_NOEXCEPT_OR_NOTHROW:
		Event(START_DOCUMENT),
		version_(version),
		encoding_(encoding),
		standalone_(standalone)
	{}
	std::string version() const {
		return version_;
	}
	std::string encoding() const {
		return encoding_;
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
BOOST_MOVABLE_BUT_NOT_COPYABLE(ElementEvent)
public:
	ElementEvent(EvenType type, const std::string& uri, const std::string& localName) BOOST_NOEXCEPT_OR_NOTHROW:
		Event(type),
		uri_(uri),
		localName_(localName)
	{}
	inline std::string uri() const {
		return uri_;
	}
	inline std::string localName() const {
		return localName_;
	}
private:
	const std::string uri_;
	const std::string localName_;
};

class AttributeEvent:public Event {
private:
	Attribute attr_;
public:
	AttributeEvent(const Attribute& attr) BOOST_NOEXCEPT_OR_NOTHROW:
		Event(ATTRIBUTE),
		attr_(attr)
	{}
	inline Attribute getAttribute() {
		return attr_;
	}
};

class CharactersEvent:public Event {
public:
	CharactersEvent() BOOST_NOEXCEPT_OR_NOTHROW:
		Event(CHARACTERS)
	{}
};

} // xmlevent

#endif // EVNET_HPP_INCLUDED
