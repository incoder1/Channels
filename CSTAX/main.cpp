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

StartElementEvent::~StartElementEvent() BOOST_NOEXCEPT_OR_NOTHROW
{
}


class XMLEventReader {
private:
	typedef std::vector<boost::regex> lang_t;
public:
	explicit XMLEventReader(io::SReadChannel src, io::SConverter conv) BOOST_NOEXCEPT_OR_NOTHROW;
	bool hasNext();
	SXMLEvent peek();
	SXMLEvent next();
private:
	io::Reader<std::string> reader_;
	std::string buff_;
};

XMLEventReader::XMLEventReader(io::SReadChannel src, io::SConverter conv) BOOST_NOEXCEPT_OR_NOTHROW:
		reader_(src,io::new_byte_byffer(128),conv)
{
}


SXMLEvent XMLEventReader::next()
{
	bool hasResult = false;
	while(!hasResult) {
		if(buff_.empty()) {
			buff_.append(reader_.read());
			if(buff_.empty()) {
				hasResult = true;
				std::cout<<"No more data"<<std::endl;
			}
		}
	}
	}
	return SXMLEvent(new StartElementEvent(std::vector<Attribute>()));
}


int main()
{

			//charset_console_sample();

		const char *xml = "<\?xml version=\"1.0\"?> \
		                      <!-- a comment --> \
		                      <hello> \
		                         <who val=\"world\"/> \
							  </hello>";
		io::SReadChannel src(new io::DataChannel((void*)xml, std::strlen(xml)));
		XMLEventReader lexer(src,io::char_empty_converter());;

    cout << "Hello world!" << endl;
    return 0;
}
