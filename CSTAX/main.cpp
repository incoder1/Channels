#include <iostream>

using namespace std;

enum XMLEventType {
    START_ELEMENT,
    END_ELEMENT,
    ATTRIBUTE,
    ELEMENT_TEXT,
    XML_DEF,
    DTD,
    COMMENT_START,
    COMMEND_END,
    SCHEMA
};

class XMLEventReader;

class XMLEvent {
public:
	XMLEventType type() const {
		return type_;
	}
protected:
	explicit XMLEvent(XMLEventType type) BOOST_NOEXCEPT_OR_NOTHROW;
	virtual ~XMLEvent() BOOST_NOEXCEPT_OR_NOTHROW = 0;
	friend class XMLEventReader;
private:
	XMLEventType type_;
};

XMLEvent::XMLEvent(XMLEventType type) BOOST_NOEXCEPT_OR_NOTHROW:
type_(type)
{}

XMLEvent::~XMLEvent() BOOST_NOEXCEPT_OR_NOTHROW
{}

typedef boost::shared_ptr<XMLEvent> SXMLEvent;

struct Attribute {
	const char *name;
	const char *value;
};

class StartElementEvent: public XMLEvent {
private:
	std::vector<Attribute> attrs_;
public:
	StartElementEvent(std::vector<Attribute> attrs) BOOST_NOEXCEPT_OR_NOTHROW:
		XMLEvent(START_ELEMENT),
		attrs_(attrs)
	{}
	virtual ~StartElementEvent() BOOST_NOEXCEPT_OR_NOTHROW;
	inline unsigned short attrCount() const {
		attrs_.size();
	}
	inline const Attribute* attrs() const {
		&attrs_[0];
	}
};

StartElementEvent::~StartElementEvent() BOOST_NOEXCEPT_OR_NOTHROW {
}



int main(int argc,const char **argv) {
	return 0;
}
