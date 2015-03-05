#ifndef QNAME_HPP_INCLUDED
#define QNAME_HPP_INCLUDED

#include <string>
#include <boost/detail/noexcept.hpp>

namespace xml {

class Qname {
public:
	QName(const std::string& namespaceURI,const std::string& localPart,const std::string& prefix) BOOST_NOEXCEPT_OR_NOTHROW:

	{}
private:
 const std::string namespaceURI_;

};

} // namesapce xml
#endif // QNAME_HPP_INCLUDED
