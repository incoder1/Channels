#ifndef __IO_SMALLOBJECT_HPP_INCLUDED__
#define __IO_SMALLOBJECT_HPP_INCLUDED__

#include "channels_config.h"

#include <stdint.h>
#include <stdexcept>

#include <boost/atomic.hpp>
#include <boost/shared_ptr.hpp>

namespace io {

/// !\brief Abstract class, which offspring will use segregate storage memory allocation.
/// Simple segregate storage allocation will be applied only for objects with size less or equals 128 byte.
/// If object is bigger then 128 bytes default new/delete allocation from C++ standard library will be used
class CHANNEL_PUBLIC object {
  // copying of abstract class is not allowed
  BOOST_MOVABLE_BUT_NOT_COPYABLE(object)
protected:
	/**
	 * Direct instanciating is not allowed
	 * Implementor must call inherit basic constructor call according C++ specification
	 * \throw never throws
	 */
	object() BOOST_NOEXCEPT_OR_NOTHROW;

public:
	/**
	 * Pure-virtual destructor, implementor must provide an not't throwing destructor
	 * \throw never throws
	 */
	virtual ~object() BOOST_NOEXCEPT_OR_NOTHROW = 0;

	/**
	 * Returns hash of object, original function returns \code this pointer value.
	 * \return object hash
	 * \throw never throws
	 */
	virtual std::size_t hash() const BOOST_NOEXCEPT_OR_NOTHROW;

	/**
	 * Ovverides system memory allocator to the sorted single segregate storage allocation.
	 * If implementor size is lager then 128 bytes, default new will be used
	 * \param size the size of implementor in bytes
	 * \return pointer on new allocated object
	 * \throw std::bad_alloc if memory can not be allocated
	 */
	void* operator new(std::size_t size) throw(std::bad_alloc);

	/**
	 * Ovverides system memory deleter to the sorted single segregate storage deleter.
	 * If implementor size is lager then 128 bytes, default delete will be used
	 * \param ptr pointer on allocated object
	 * \param size size in bytes of allocated object
	 * \throw never throws
	 */
	void operator delete(void *ptr,std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW;
};


#ifndef DECLARE_PTR_T
#	define DECLARE_PTR_T(TYPE) typedef boost::shared_ptr<TYPE> S##TYPE
#endif // DECLARE_SPTR_T

} // namespace io

#endif // __IO_SMALLOBJECT_HPP_INCLUDED__
