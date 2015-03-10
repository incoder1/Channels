#include "xmlgrammar.hpp"

namespace xml {


using namespace boost::xpressive;

const sregex grm::TRUE_EXP = bos >> "yes" | "YES" | "true" | "TRUE" >> eos;  // (yes|YES|TRUE|true)

const sregex grm::START_DOCUMENT_EXP = bos >> "xml" >> space >> -+_ >> eos;
const sregex grm::DOCUMENT_VERSION_EXP =  (s1 = "version=") >> '"' >> (s2 = -+(_d | '.')) >> '"'; // (version=")((\d|\.)*?)"
const sregex grm::DOCUMENT_ENCODING_EXP = (s1 = "encoding=") >> '"' >> (s2 = -+_) >> '"'; // "(encoding=")(.+)"
const sregex grm::DOCUMENT_STENDALONE_EXP =  (s1 = "standalone=") >>'"'>> (s2 = -+_) >>'"'; // (standalone=")(.+?)"

const sregex grm::INSTRUCTION_TYPE_EXP = (s1 = "type=") >> '"' >> (s2 = -+_) >> '"';
const sregex grm::INSTRUCTION_HREF_EXP = (s1 = "href=") >> '"' >> (s2 = -+_) >> '"';

const sregex grm::ELEMENT_NAME_EXP = -+~(set = '=' , '"' , ' ', '\t', '\n', '\r') >> _s;
const sregex grm::ATTRIBUTE_EXP =  (s1 = -+~(set= '=','"',' ','\t','\n','\r')) >> '"' >> (s2 = -+_) >> '"';

} // namespace xml
