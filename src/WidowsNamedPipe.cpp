#include "prchdrs.h"
#include "helpers.hpp"
#include "WidowsNamedPipe.hpp"

namespace io {

static const DWORD MAX_PIPE_BUFFER = 1024; // 1 K is max

SReadWriteChannel open_named_pipe(const char* pipeName)
{
	HANDLE hPipe = ::CreateFileA(pipeName,
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					0);
	validate_io(INVALID_HANDLE_VALUE != hPipe,"Can't open named pipe");
	return SReadWriteChannel(new FileChannel(hPipe, GENERIC_READ | GENERIC_WRITE, true));
}

SReadWriteChannel create_named_pipe(const char* pipeName)
{
	HANDLE hPipe =
	::CreateNamedPipeA(pipeName,
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS,
			1,
			MAX_PIPE_BUFFER,
			MAX_PIPE_BUFFER,
			NMPWAIT_USE_DEFAULT_WAIT,
			NULL);
	validate_io(INVALID_HANDLE_VALUE != hPipe, "Can't create named pipe");
	return SReadWriteChannel(new FileChannel(hPipe, GENERIC_READ | GENERIC_WRITE, true));
}

} // namespace io
