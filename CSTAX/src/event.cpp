#include "event.hpp"

namespace xml {

// Event
Event::Event(EvenType type) BOOST_NOEXCEPT_OR_NOTHROW:
		io::object(),
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

// ProcessingInstructionEvent
ProcessingInstructionEvent::ProcessingInstructionEvent(const std::string& type, const std::string& href) BOOST_NOEXCEPT_OR_NOTHROW:
	Event(PROCESSING_INSTRUCTION),
	type_(type),
	href_(href)
{}

ProcessingInstructionEvent::~ProcessingInstructionEvent() BOOST_NOEXCEPT_OR_NOTHROW
{
}


} // namespace xml
