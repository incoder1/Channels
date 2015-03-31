#include "xmlgrammar.hpp"

namespace xml {

using namespace boost::xpressive;

#define DEFINE_EXPR(ID,EXPR) const grm::regex_t grm::ID = EXPR

// (yes|YES|TRUE|true)
DEFINE_EXPR(TRUE_EXP, bos >> "yes" | "YES" | "true" | "TRUE" >> eos);
// document prolog
DEFINE_EXPR(START_DOCUMENT_EXP, bos>> -*_ >> "<?xml" >> space >> -+_  >> "?>" >> eos);
DEFINE_EXPR(DOCUMENT_VERSION_EXP, (s1 = "version=") >> '"' >> (s2 = -+(_d | '.')) >> '"'); // (version=")((\d|\.)*?)"
DEFINE_EXPR(DOCUMENT_ENCODING_EXP, (s1 = "encoding=") >> '"' >> (s2 = -+_) >> '"'); // "(encoding=")(.+)"
DEFINE_EXPR(DOCUMENT_STENDALONE_EXP,  (s1 = "standalone=") >>'"'>> (s2 = -+_) >>'"'); // (standalone=")(.+?)"
// Processing instruction
DEFINE_EXPR(PROCESSING_INSTRUCTION_EXP, bos >> -*_ >> "<?" >> +~_s >> -+_  >> "?>" >> eos);
DEFINE_EXPR(INSTRUCTION_TYPE_EXP, (s1 = "type=") >> '"' >> (s2 = -+_) >> '"');
DEFINE_EXPR(INSTRUCTION_HREF_EXP, (s1 = "href=") >> '"' >> (s2 = -+_) >> '"');

DEFINE_EXPR(ELEMENT_NAME_EXP,-+~(set = '=' , '"' , ' ', '\t', '\n', '\r') >> _s);
DEFINE_EXPR(ATTRIBUTE_EXP, (s1 = -+~(set= '=','"',' ','\t','\n','\r')) >> '"' >> (s2 = -+_) >> '"');

#undef DEFINE_EXPR // undefine decl

} // namespace xml
