#include "prchdrs.h"
#include <channels_config.h>

#ifdef PLATFORM_WINDOWS

#include "WindowsFile.hpp"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE; // successful
}
#endif // PLATFORM_WINDOWS


