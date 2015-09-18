#ifndef __COMPAT_HPP_INCLUDED__
#define __COMPAT_HPP_INCLUDED__

#include <iosfwd>                           // streamsize, seekdir
#include <boost/iostreams/categories.hpp>   // seekable_device_tag

#include "text.hpp"

namespace io {

namespace compat {

	/// Compatibility layer between stdlib streams and channels
	template<typename _reader, std::size_t def_buf_size = 128>
	class synch_read_channel_device  {
	public:
		typedef _char_t char_type;
		typedef boost::iostreams::input category;
		explicit synch_read_channel_device(const _reader& reader):
			reader_(reader),
			buffer_(byte_buffer::heap_buffer(def_buf_size))
		{}
		std::streamsize read(char_type* s, std::streamsize n) {
			std::streamsize result;
			byte_buffer buf = byte_buffer::wrap_array(s,static_cast<std::size_t>(n));
			return static_cast<std::streamsize>(reader.read(buff));
		}
	private:
		_reader reader_;
	};

} // namespace compat

} // namespace io

#endif // __COMPAT_HPP_INCLUDED__
