#ifndef CHANNESL_SMALLOBJECT_HPP_INCLUDED
#define CHANNESL_SMALLOBJECT_HPP_INCLUDED

#include <channels_config.h>

#include <stdexcept>
#include <boost/move/move.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace io {

/// !\brief Abstract class, which offspring will use segregate storage memory allocation.
/// WARNING! This working only in case that you object is small i.e. size is less then 64 bytes
/// If object is bigger then 128 bytes default new/delete will be used
class CHANNEL_PUBLIC object {
  BOOST_MOVABLE_BUT_NOT_COPYABLE(object)
public:
	object() BOOST_NOEXCEPT_OR_NOTHROW;
	virtual std::size_t hash() const;
	virtual bool equal(object* const obj) const;
	virtual ~object() BOOST_NOEXCEPT_OR_NOTHROW = 0;
	void* operator new(std::size_t size) throw(std::bad_alloc);
	void operator delete(void *ptr,std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW;
};

} // namespace io

#endif // CHANNESL_SMALLOBJECT_HPP_INCLUDED
