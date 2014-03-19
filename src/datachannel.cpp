#include "datachannel.hpp"

namespace io {

	DataChannel::DataChannel(void *data, std::size_t size) BOOST_NOEXCEPT_OR_NOTHROW:
		data_(wrap_array(reinterpret_cast<uint8_t*>(data),size))
	{}

	DataChannel::DataChannel(const byte_buffer& buff) BOOST_NOEXCEPT_OR_NOTHROW:
		data_(buff)
	{}

	DataChannel::~DataChannel() BOOST_NOEXCEPT_OR_NOTHROW
	{}

	std::size_t DataChannel::read(byte_buffer& buffer) throw(io_exception)
	{
		std::size_t offset = buffer.capacity();
		if(&data_.position() + offset >= &data_.end()) {
			offset = &data_.last() - &data_.position();
		}
		buffer.put(&data_.position(), offset);
		data_.move(offset);
		return offset;
	}

	std::size_t DataChannel::write(const byte_buffer& buffer) throw(io_exception)
	{
		return data_.put(buffer);
	}

	void DataChannel::seek(std::size_t offset, MoveMethod method) throw(io_exception)
	{
		switch(method) {
		case FROM_BEGIN:
			data_.flip();
			data_.move(offset);
			break;
		case FROM_CURRENT_POSITION:
			data_.move(offset);
			break;
		case FROM_END:
			data_.flip();
			data_.move(data_.capacity() - offset);
			break;
		default:
			// possible if used outside of C++
			boost::throw_exception(io_exception("Invalid method value"));
		}
	}

} // namespace io