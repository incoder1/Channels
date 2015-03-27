#ifndef CSTAX_XML_GRAMMAR_HPP_INCLUDED
#define CSTAX_XML_GRAMMAR_HPP_INCLUDED

#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_algorithms.hpp>

namespace xml {

#define DECLARE_EXPR(CONSTANT,ACCESSOR)\
		private:\
			static const regex_t CONSTANT;\
		public:\
			static inline const regex_t ACCESSOR()\
			{return CONSTANT;}

class grm {
public:
	typedef boost::xpressive::sregex regex_t;
// yes or true
	DECLARE_EXPR(TRUE_EXP,true_exp)
// start document
	DECLARE_EXPR(START_DOCUMENT_EXP,start_doc_exp)
	DECLARE_EXPR(DOCUMENT_VERSION_EXP,doc_version_exp)
	DECLARE_EXPR(DOCUMENT_ENCODING_EXP,doc_encoding_exp)
	DECLARE_EXPR(DOCUMENT_STENDALONE_EXP,doc_stendalone_exp)
//processing instruction
	DECLARE_EXPR(INSTRUCTION_TYPE_EXP,instr_type_exp)
	DECLARE_EXPR(INSTRUCTION_HREF_EXP,instr_href_exp)
// element
	DECLARE_EXPR(ELEMENT_NAME_EXP,element_name_exp)
	DECLARE_EXPR(ATTRIBUTE_EXP,atribute_exp)
}; // struct grm

#undef  DECLARE_EXPR // DECLARE_EXPR

} // namespace xml
#endif // CSTAX_XML_GRAMMAR_HPP_INCLUDED
