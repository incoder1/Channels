#include "convert.hpp"

namespace io {

// charset_exception
charset_exception::charset_exception(const std::string& message):
	std::exception(),
	msg_(message)
{}

const char* charset_exception::what() const BOOST_NOEXCEPT
{
	return msg_.c_str();
}

// Charset

Charset::Charset(size_t id, const std::string& name, const size_t charSize) BOOST_NOEXCEPT_OR_NOTHROW:
	id_(id),
    name_(name),
    charSize_(charSize)
{}

const size_t Charset::id() const BOOST_NOEXCEPT_OR_NOTHROW {
	return id_;
}

const std::string Charset::name() const BOOST_NOEXCEPT_OR_NOTHROW
{
	return name_;
}

const size_t Charset::charSize() const BOOST_NOEXCEPT_OR_NOTHROW {
	return charSize_;
}

bool Charset::equal(const Charset* oth) const
{
	bool result = false;
	if(NULL != oth) {
		result = id_ == (oth->id_);
	}
	return result;
}


// Converter
Converter::~Converter()
{
}

} // namespace io
