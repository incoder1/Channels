#include "event.hpp"

namespace xml {

// Event
Event::Event(EvenType type) BOOST_NOEXCEPT_OR_NOTHROW:
		io::object(),
		boost::enable_shared_from_this<Event>(),
		type_(type)
{}


// DocumentEvent
DocumentEvent::DocumentEvent(const std::string& version, const std::string& encoding, bool standalone) BOOST_NOEXCEPT_OR_NOTHROW:
		Event(START_DOCUMENT),
		version_(version),
		encoding_(encoding),
		standalone_(standalone)
{}

DocumentEvent::~DocumentEvent() BOOST_NOEXCEPT_OR_NOTHROW
{
}

//ElementEvent

ElementEvent::ElementEvent(EvenType type, const std::string& uri, const std::string& localName) BOOST_NOEXCEPT_OR_NOTHROW:
		Event(type),
		uri_(uri),
		localName_(localName)
{}

ElementEvent::~ElementEvent() BOOST_NOEXCEPT_OR_NOTHROW
{
}


} // namespace xml
