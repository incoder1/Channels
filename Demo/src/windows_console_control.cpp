#include "windows_console_control.hpp"

namespace gui {

WindowsConsoleControl::WindowsConsoleControl():
	richEdit_(NULL),
	module_(::LoadLibraryW(L"Msftedit.dll"))
{
}

void WindowsConsoleControl::inject(HWND parent) const {
	richEdit_ = ::CreateWindowExW(
				WS_EX_CLIENTEDGE, MSFTEDIT_CLASS, NULL,
				ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_TABSTOP,
				0, 0, 10, 10,
				parent, NULL, ::GetModuleHandle(0), NULL);
	::ShowWindow(richEdit_, SW_SHOWNORMAL);
	::UpdateWindow(richEdit_);
}

void WindowsConsoleControl::resize(DWORD width, DWORD heigth) const {
	::MoveWindow(richEdit_, 0, 0, width, heigth,TRUE);
}



//typedef DWORD CALLBACK (*EDITSTREAMCALLBACK)(DWORD,LPBYTE, LONG, LONG*);

static DWORD CALLBACK EditStreamCallback(DWORD_PTR dwCookie,
                                  LPBYTE lpBuff,
                                  LONG cb,
                                  PLONG pcb)
{
    HANDLE hFile = (HANDLE)dwCookie;
    if (::ReadFile(hFile, lpBuff, cb, (DWORD *)pcb, NULL))
    {
        return 0;
    }
    return -1;
}

//typedef struct _editstream {
//    DWORD dwCookie;
//    DWORD dwError;
//    EDITSTREAMCALLBACK pfnCallback;
//} EDITSTREAM;


void WindowsConsoleControl::redirectStdOut() const {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	EDITSTREAM es = { 0 };
	es.pfnCallback = EditStreamCallback;
	es.dwCookie    = (DWORD_PTR)hOut;
	LRESULT msg = ::SendMessage(richEdit_, EM_STREAMIN, SF_RTF, (LPARAM)&es);
	bool res = msg && !es.dwError;
}

WindowsConsoleControl::~WindowsConsoleControl()
{
	::FreeLibrary(module_);
}

} // namespace gui
