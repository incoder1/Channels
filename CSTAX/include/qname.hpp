#ifndef QNAME_HPP_INCLUDED
#define QNAME_HPP_INCLUDED

#include <string>
#include <boost/move/move.hpp>

namespace xml {

class Qname {
public:
	QName(const std::string& namespaceURI,const std::string& localPart,const std::string& prefix) BOOST_NOEXCEPT_OR_NOTHROW:
		namespaceURI_(namespaceURI_),
		localPart_(localPart),
		prefix_(prefix)
	{}
	inline std::string namespaceURI() const {
		return namespaceURI_;
	}
	inline std::string localPart() const {
		return localPart_;
	}
	inline std::string prefix() const {
		return prefix_;
	}
private:
 const std::string namespaceURI_;
 const std::string localPart_;
 const std::string prefix_;
};

} // namesapce xml
#endif // QNAME_HPP_INCLUDED
