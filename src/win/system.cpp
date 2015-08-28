#include "prchdrs.h"

#include "system.hpp"

#include <Windows.h>

namespace sys {

uint64_t CHANNEL_PUBLIC available_physical_mememory() BOOST_NOEXCEPT_OR_NOTHROW
{
	::MEMORYSTATUSEX memstat;
	::GlobalMemoryStatusEx(&memstat);
	return memstat.ullAvailPhys;
}

uint64_t CHANNEL_PUBLIC toal_physical_memory() BOOST_NOEXCEPT_OR_NOTHROW
{
	::MEMORYSTATUSEX memstat;
	::GlobalMemoryStatusEx(&memstat);
	return memstat.ullTotalPhys;
}

unsigned long CHANNEL_PUBLIC available_logical_cpus() BOOST_NOEXCEPT_OR_NOTHROW
{
	::SYSTEM_INFO sysinfo;
	::GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
}

const char* CHANNEL_PUBLIC get_temp_dir() BOOST_NOEXCEPT_OR_NOTHROW
{
	static char result[MAX_PATH+1];
	::GetTempPathA(MAX_PATH+1,result);
	return result;
}

const char* CHANNEL_PUBLIC generate_temp_file_name(const char* prefix) BOOST_NOEXCEPT_OR_NOTHROW
{
	static char result[MAX_PATH+1];
	::GetTempFileNameA(get_temp_dir(),prefix,0,result);
	return result;
}

} // namespace sys
