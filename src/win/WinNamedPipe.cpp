#include "prchdrs.h"

#include "WinChannel.hpp"
#include "namedpipe.hpp"
#include "helpers.hpp"

namespace io {

	namespace ipc {

		static const char* SYSTEM_PREFIX = "\\\\.\\pipe\\";

		inline std::string prepare_name(const char* name)
		{
			std::string result(SYSTEM_PREFIX);
			result.append(name);
			return result;
		}


		inline WinChannel* create_named_pipe(const char* name, DWORD openMode,std::size_t bufferSize,uint8_t maxInstances)
		{
			std::string pipeName = prepare_name(name);
			DWORD createMode = openMode | FILE_FLAG_FIRST_PIPE_INSTANCE;
			::HANDLE hPipe = ::CreateNamedPipeA(pipeName.c_str(), createMode, PIPE_TYPE_BYTE | PIPE_WAIT, maxInstances, bufferSize, bufferSize, 0, NULL);
			validate<std::runtime_error>(INVALID_HANDLE_VALUE != hPipe, "Can not create pipe");
			validate<std::runtime_error>(::ConnectNamedPipe(hPipe,NULL),"Can't listen clients");
			return new WinChannel(hPipe, createMode );
		}

		inline WinChannel* connect_named_pipe(const char* name, DWORD connectMode) {
			std::string pipeName = prepare_name(name);
			::HANDLE hPipe = ::CreateFileA(pipeName.c_str(), connectMode, 0, NULL, OPEN_EXISTING, 0, NULL);
			validate<std::runtime_error>(INVALID_HANDLE_VALUE != hPipe, std::string("Can not connect to")+name);
			return new WinChannel(hPipe,connectMode);
		}

		SReadWriteChannel CHANNEL_PUBLIC create_duplex_named_pipe(const char* name,std::size_t bufferSize, uint8_t maxInstances)
		{
			return SReadWriteChannel(create_named_pipe(name, PIPE_ACCESS_DUPLEX, bufferSize, maxInstances));
		}

		SReadWriteChannel CHANNEL_PUBLIC connect_duplex_named_pipe(const char* name)
		{
			return SReadWriteChannel(connect_named_pipe(name,GENERIC_READ | GENERIC_WRITE));
		}

	} // namespace ipc

} // nemesapce io

