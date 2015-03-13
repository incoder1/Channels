#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <smallobject.hpp>

#include <boost/move/move.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

namespace gui {

class object:public virtual boost::enable_shared_from_this<object>, public virtual io::SmallObject
{
	BOOST_MOVABLE_BUT_NOT_COPYABLE(object)
	public:
		object() BOOST_NOEXCEPT_OR_NOTHROW;
		virtual std::size_t hash() const;
		virtual ~object() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

typedef boost::shared_ptr<object> SObject;

} // namespace gui

#endif // OBJECT_HPP_INCLUDED
