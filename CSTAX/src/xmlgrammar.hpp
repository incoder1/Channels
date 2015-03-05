#ifndef CSTAX_XML_GRAMMAR_HPP_INCLUDED
#define CSTAX_XML_GRAMMAR_HPP_INCLUDED

#include <stdint.h>

#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_algorithms.hpp>

namespace xml {


class grm {

public:

static inline const boost::xpressive::sregex doc_ver_exp() {
	return DOCUMENT_VERSION_EXP;
}

static inline const boost::xpressive::sregex doc_encd_exp() {
	return DOCUMENT_ENCODING_EXP;
}

static inline const boost::xpressive::sregex doc_stendalone_exp() {
	return DOCUMENT_STENDALONE_EXP;
}

static inline const boost::xpressive::sregex true_exp() {
	return TRUE_EXP;
}

static inline const boost::xpressive::sregex xml_document_exp() {
	return START_DOCUMENT_EXP;
}

private:

static const boost::xpressive::sregex START_DOCUMENT_EXP;
static const boost::xpressive::sregex DOCUMENT_VERSION_EXP;
static const boost::xpressive::sregex DOCUMENT_ENCODING_EXP;
static const boost::xpressive::sregex DOCUMENT_STENDALONE_EXP;
static const boost::xpressive::sregex TRUE_EXP;

}; // struct grm

} // namespace xml
#endif // CSTAX_XML_GRAMMAR_HPP_INCLUDED