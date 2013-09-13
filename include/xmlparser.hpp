#ifndef XMLPARSER_HPP_INCLUDED
#define XMLPARSER_HPP_INCLUDED

#include <string>
#include <algorithm>
#include <list>

#include "channels.hpp"
#include "charbuffers.hpp"
#include "convert.hpp"

namespace xml {

template<class string>
class Attribute {
private:
	string name_;
	string value_;
public:
explicit Attribute(const string& name, const string &value) BOOST_NOEXCEPT_OR_NOTHROW:
	name_(name),
	      value_(value)
	{}
	inline string getName() const {
		return name_;
	}
	string getValue() const {
		return value_;
	}
	inline void setName(const string& name) {
		name_ =  name;
	}
	inline void setValue(const string& value) {
		value_ = value;
	}
	bool equal(const Attribute& attr) {
		return (name_ == attr.name_) && (value_ == attr.value_);
	}
};


typedef enum {
    SIMPLE, COMPLEX
} NodeType;


template<class string>
class XMLNode {
public:
	typedef Attribute<string> *PAttribute;
	typedef typename std::list<PAttribute> AttributeList;
	typedef typename std::list< XMLNode* > NodeList;
private:
	string name_;
	string nameSpace_;
	string value_;
	AttributeList attributes_;
	NodeType type_;
	NodeList embedded_;
private:
	inline void checkAndUpdateType() {
		if(attributes_.empty() && embedded_.empty()) {
			type_ = SIMPLE;
		} else {
			type_ = COMPLEX;
		}
	}
public:
explicit XMLNode(const string& name, const string& nameSpace, const string& value) BOOST_NOEXCEPT_OR_NOTHROW:
	name_(name),
	      nameSpace_(nameSpace_),
	      value_(value),
	      type_(SIMPLE),
	      embedded_()
	{}
	NodeType type() const {
		return type_;
	}
	string name() const {
		return name_;
	}
	string nameSpace() const {
		return nameSpace_;
	}
	bool hasValue() const {
		return !value_.empty();
	}
	string getValue() const {
		return value_;
	}
	void setValue(string value) {
		value_ = value;
	}
	AttributeList attributes() const {
		return attributes_;
	}
	virtual void addAttribute(PAttribute attr) {
		if(NULL != attr) {
			type_ = COMPLEX;
			attributes_.push_back(attr);
			checkAndUpdateType();
		}
	}
	void removeAttribute(PAttribute attr) {
		if(NULL != attr) {
			// TODO: Move to the boost container
			PAttribute deleted = attributes_.remove(attr);
			if(NULL != deleted) {
				delete deleted;
			}
		}
	}
};




} // namespace xml

#endif // XMLPARSER_HPP_INCLUDED
