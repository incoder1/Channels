#ifndef __ASYNCHRONOUS_HPP_INCLUDED__
#define __ASYNCHRONOUS_HPP_INCLUDED__

#include "smallobject.hpp"
#include "errors.hpp"
#include "bytebuffer.hpp"

#include <boost/move/move.hpp>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>

namespace io {

typedef boost::system::error_code error_code;

namespace detail {

template<class _async_channel_t>
class basic_async_handler {
	BOOST_COPYABLE_AND_MOVABLE(asynh_handler_f<_async_channel_t>);
public:
	typedef typename smart_ptr_type<_async_channel_t>::smart_ptr_t s_async_channel_t;
	typedef typename boost::function<void(s_async_channel_t,const error_code&,std::size_t,byte_buffer&)> functor_t;
	typedef typename functor_t::result_type result_type;
	basic_async_handler(BOOST_COPY_ASSIGN_REF(functor_t) f) BOOST_NOEXCEPT_OR_NOTHROW:
		functor_(f)
	{}
	void operator()(const s_async_channel_t& ch, const error_code& err, std::size_t transfered, byte_buffer& buff) const {
		buff.move(transfered);
		functor_(ch,err,transfered,buff);
	}
private:
	const functor_t functor_;
};

} // namesapce detail

class CHANNEL_PUBLIC AsyncChannel:public virtual object
{
	BOOST_MOVABLE_BUT_NOT_COPYABLE(AsyncChannel)
protected:
	AsyncChannel();
public:
	typedef detail::basic_async_handler<AsyncChannel> handler_f;
	virtual void send(std::size_t position,byte_buffer& buffer,const handler_f& handler) = 0;
	virtual void receive(std::size_t position,std::size_t maxSize,const handler_f& handler) = 0;
	virtual ~AsyncChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
};

DECLARE_PTR_T(AsyncChannel);

} // namesapce io

#endif // __ASYNCHRONOUS_HPP_INCLUDED__
