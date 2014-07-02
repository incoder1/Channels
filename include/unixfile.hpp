#ifndef CHANNELS_UNIXFILE_HPP_INCLUDED
#define CHANNELS_UNIXFILE_HPP_INCLUDED

#include "abstractfile.hpp"

namespace io {

/**
 * ! \brief Unix depended blocking file Channel implementation.
 */
class CHANNEL_PUBLIC FileChannel:public virtual ReadWriteChannel {
public:
private:
};

} // namespace io

#endif // CHANNELS_UNIXFILE_HPP_INCLUDED
