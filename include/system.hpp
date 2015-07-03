#ifndef __SYSTEM_HPP_INCLUDED__
#define __SYSTEM_HPP_INCLUDED__

#include "channels_config.h"

namespace sys {

	uint64_t CHANNEL_PUBLIC available_physical_mememory() BOOST_NOEXCEPT_OR_NOTHROW;

	uint64_t CHANNEL_PUBLIC toal_physical_memory() BOOST_NOEXCEPT_OR_NOTHROW;

	unsigned long CHANNEL_PUBLIC available_logical_cpus() BOOST_NOEXCEPT_OR_NOTHROW;

	const char* CHANNEL_PUBLIC get_temp_dir() BOOST_NOEXCEPT_OR_NOTHROW;

	const char* CHANNEL_PUBLIC generate_temp_file_name(const char* prefix) BOOST_NOEXCEPT_OR_NOTHROW;

} // namesapce sys

#endif // __SYSTEM_HPP_INCLUDED__
