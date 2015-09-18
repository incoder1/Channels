#ifndef __ASYNCHRONOUS_HPP_INCLUDED__
#define __ASYNCHRONOUS_HPP_INCLUDED__

#include "smallobject.hpp"
#include "errors.hpp"
#include "bytebuffer.hpp"

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>

namespace io {

	typedef boost::system::error_code error_code;

	class AsynchronousChannel; DECLARE_PTR_T(AsynchronousChannel);
	typedef boost::function<void(SAsynchronousChannel,const error_code&,std::size_t,byte_buffer&)> asynh_handler_f;

	class CHANNEL_PUBLIC AsynchronousChannel:public virtual object {
	private:
		BOOST_MOVABLE_BUT_NOT_COPYABLE(AsynchronousChannel)
	protected:
		AsynchronousChannel();
	public:
		virtual void send(std::size_t position,byte_buffer& buffer,const asynh_handler_f& handler) BOOST_NOEXCEPT_OR_NOTHROW = 0;
		virtual ~AsynchronousChannel() BOOST_NOEXCEPT_OR_NOTHROW = 0;
	};


} // namesapce io

#endif // __ASYNCHRONOUS_HPP_INCLUDED__
