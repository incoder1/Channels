#ifndef CHANNESL_SMALLOBJECTPOOL_HPP_INCLUDED
#define CHANNESL_SMALLOBJECTPOOL_HPP_INCLUDED

#include <channels_config.h>
#include <stdexcept>
#include <cassert>

#include <boost/throw_exception.hpp>
#include <boost/pool/pool.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread/once.hpp>
#include <boost/thread/mutex.hpp>

#include <boost/atomic.hpp>

namespace io {

/// !\brief Abstract class, which offspring will use segregate storage memory allocation.
/// WARNING! This working only in case that you object is small i.e. size is less then 64 bytes
/// If object is bigger then 64 bytes default new/delete will be used
class CHANNEL_PUBLIC SmallObject {
protected:
	SmallObject() BOOST_NOEXCEPT_OR_NOTHROW
	{}
public:
	SmallObject(const SmallObject &c)
	{}
	SmallObject& operator=(const SmallObject& c) {
		return *this;
	}
	virtual ~SmallObject() BOOST_NOEXCEPT_OR_NOTHROW = 0;
	void * operator new(std::size_t size) throw(std::bad_alloc);
	void operator delete(void* ptr, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW;
};


}
#endif // CHANNESL_SMALLOBJECTPOOL_HPP_INCLUDED
