#include "prchdrs.h"
#include <Windows.h>
#include "system.hpp"

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

} // namespace sys
