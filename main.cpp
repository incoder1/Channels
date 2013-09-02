#include <channels_config.h>

//namespace io {
//
//class Console {
//private:
//	HANDLE cin_;
//	HANDLE cout_;
//	HANDLE cerr_;
//public:
//	Console():
//		cout_(NULL)
//	{
//		cout_ = ::GetStdHandle(STD_OUTPUT_HANDLE);
//		cin_ =  ::GetStdHandle(STD_INPUT_HANDLE);
//		cerr_ = ::GetStdHandle(STD_ERROR_HANDLE);
//	}
//	PWriteChannel out() throw(std::runtime_error) {
//		if(INVALID_HANDLE_VALUE == cout_) {
//			throw std::runtime_error("Can not access to the console screen buffer");
//		}
//		return PWriteChannel(new FileChannel(cout_));
//	}
//	PReadChannel in() throw(std::runtime_error) {
//		if(INVALID_HANDLE_VALUE == cin_) {
//			throw std::runtime_error("Can not access to the console screen buffer");
//		}
//		return PReadChannel(new FileChannel(cin_));
//	}
//	PWriteChannel err() throw(std::runtime_error) {
//		if(INVALID_HANDLE_VALUE == cerr_) {
//			throw std::runtime_error("Can not access to the console screen buffer");
//		}
//		return PWriteChannel(new FileChannel(cerr_));
//	}
//};
//
//} // namespace io

#ifdef PLATFROM_WINDOWS
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
	return TRUE; // succesful
}
#endif // PLATFROM_WINDOWS


