#ifndef EVNET_HPP_INCLUDED
#define EVNET_HPP_INCLUDED

// boost
#include <boost/shared_ptr.hpp>
#include <boost/flyweight.hpp>

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

class Event {
protected:
	explicit Event(EvenType type) BOOST_NOEXCEPT_OR_NOTHROW;
public:
	EvenType type() const {
		return type_.get();
	}
	virtual ~Event() BOOST_NOEXCEPT_OR_NOTHROW;
private:
	boost::flyweights::flyweight<EvenType> type_;
};

class DocumentEvent:public Event {
public:
	DocumentEvent(const std::string& version,const std::string& encoding, bool standalone) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~DocumentEvent() BOOST_NOEXCEPT_OR_NOTHROW;
	const std::string version() const {
		return version_.get();
	}
	const std::string encoding() const {
		return encoding_.get();
	}
	bool standalone() const {
		return standalone_.get();
	}
private:
	boost::flyweights::flyweight<std::string> version_;
	boost::flyweights::flyweight<std::string> encoding_;
	boost::flyweights::flyweight<bool> standalone_;
};

class ProcessingInstructionEvent:public Event {
public:
	ProcessingInstructionEvent(const std::string& type, const std::string& href) BOOST_NOEXCEPT_OR_NOTHROW;
	~ProcessingInstructionEvent() BOOST_NOEXCEPT_OR_NOTHROW;
	inline const std::string instType() const {
		return type_.get();
	}
	inline const std::string href() const {
		return href_.get();
	}
private:
	boost::flyweights::flyweight<std::string> type_;
	boost::flyweights::flyweight<std::string> href_;
};


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


typedef boost::shared_ptr<DocumentEvent> SDocumentEvent;
typedef boost::shared_ptr<ProcessingInstructionEvent> SProcessingInstructionEvent;
//typedef boost::shared_ptr<ElementEvent> SElementEvent;

namespace detail {

	template<class detail_ptr>
	class up_cast
	{
	private:
		typedef typename detail_ptr::element_type UpType;
	public:
		static inline detail_ptr cast(SEvent ev)
		{
			return boost::dynamic_pointer_cast<UpType,Event>(ev);
		}
	};
} // namespace detail

template<class UpEventPtr>
UpEventPtr up_cast(SEvent ev) {
	return detail::up_cast<UpEventPtr>::cast(ev);
}

} // xmlevent

#endif // EVNET_HPP_INCLUDED
