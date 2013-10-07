#ifndef _CHARSET_HPP_INCLUDED_
#define _CHARSET_HPP_INCLUDED_

#include <stdexcept>

namespace io {

/**
 * ! \brief Template for representing character set (code page) identifier.
 * Different code-page converter engines uses different char-set identifier types.
 * For instance Win32 API uses unsigned integer and libiconv uses string.
 */
template<typename _CharsetT, class equal_functor>
class charset {
private:
	_CharsetT value_;
public:
	explicit charset(_CharsetT name) BOOST_NOEXCEPT:
		value_(name)
	{}
	inline _CharsetT id() const {
		return value_;
	}
	bool operator==(const charset& oth) const {
		equal_functor eq;
		return eq( (value_), oth.value_ );
	}
};

typedef uint32_t bom_t;

const bom_t UTF8_BOM = 0xEFBBBF00;
const bom_t UTF16_BE_BOM = 0xFEFF0000;
const bom_t UTF16_LE_BOM = 0xFFFE0000;
const bom_t UTF32_BE_BOM = 0x0000FEFF;
const bom_t UTF32_LE_BOM = 0xFFFE0000;

/**
 * ! \brief Exception of this type trowing when character set conversation fails.
 */
class charset_exception:virtual std::runtime_error {
public:
	charset_exception(const std::string& message):
		std::runtime_error(message)
	{}
};

} // namespace io

#endif // _CHARSET_HPP_INCLUDED_
