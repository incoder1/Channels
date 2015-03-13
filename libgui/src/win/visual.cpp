#include "visual.hpp"

#ifndef GWL_USERDATA
#	define GWL_USERDATA -21
#endif // GWL_USERDATA



namespace gui {

static std::basic_string<_TCHAR> last_error_str(DWORD lastError)
{
	std::basic_string<_TCHAR> result;
	LPVOID lpMsgBuf;
	static DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD bufLen = ::FormatMessage(flags,NULL,lastError,
	                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	                   (_TCHAR*) &lpMsgBuf, 0, NULL );
	if (bufLen) {
		_TCHAR* lpMsgStr = (_TCHAR*)lpMsgBuf;
		result.append(TEXT(" - "));
		result.append(lpMsgStr, lpMsgStr+bufLen);
		::LocalFree(lpMsgBuf);
	}
	return result;
}


// Visual
const _TCHAR* Visual::BASIC_VISUAL_WINDOW_CLASS_NAME = TEXT("__LIB_GUI_BASIC_VISUAL_CLASS__");

Visual::Visual(
			Visual* const parent,
			const bounds& bounds,
			HINSTANCE hInstance,
			const _TCHAR* windowClass,
			DWORD dwStyle,
			DWORD dwExStyle):
	AbstractVisual(parent,bounds),
	hWnd_(NULL)
{
	LPWNDCLASSEX wndClass;
	if(NULL == windowClass) {
		windowClass = BASIC_VISUAL_WINDOW_CLASS_NAME;
		if(!::GetClassInfoEx(hInstance,BASIC_VISUAL_WINDOW_CLASS_NAME,wndClass)) {
			init_basic_window_class(hInstance);
		}
	} else {
		if(!::GetClassInfoEx(hInstance, windowClass, wndClass)) {
			DWORD lastError = ::GetLastError();
			show_system_error_dlg(last_error_str(lastError).c_str());
			std::string err("Windows class is not registered in current instance!");
			boost::throw_exception(std::runtime_error(err));
		}
	}

	HWND parentWnd = NULL != parent ? parent->hWnd_ : HWND_DESKTOP;
 	hWnd_ = CreateWindowEx (
           dwExStyle,                   /* Extended possibilites for variation */
		   windowClass,/* Classname */
           TEXT(""),
           dwStyle,
           0,0,0,0,
           parentWnd,
           NULL,/* No menu */
           hInstance,/* Program Instance handler */
           NULL);

	if(NULL == hWnd_) {
		DWORD lastError = ::GetLastError();
		show_system_error_dlg(last_error_str(lastError).c_str());
		boost::throw_exception(std::runtime_error("Can not create window!"));
	}

	::SetProp(hWnd_,TEXT("SELF"),(HANDLE)this);

	// Replace window proc
	ULONG_PTR callBack = ::SetClassLongPtr(
								hWnd_,
								GWLP_WNDPROC,
								(LONG_PTR)&Visual::win_proc
							);
    if( !callBack ) {
		DWORD lastError = ::GetLastError();
		show_system_error_dlg(last_error_str(lastError).c_str());
		boost::throw_exception(std::runtime_error("Can not redefine window procedure!"));
    }

}

void Visual::init_basic_window_class(HINSTANCE hInstance) {

	WNDCLASSEX wincl;

    wincl.hInstance = hInstance;
    wincl.lpszClassName = BASIC_VISUAL_WINDOW_CLASS_NAME;
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);
	wincl.lpfnWndProc = ::DefWindowProc;

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);

    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */

    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

	if(!::RegisterClassEx(&wincl)) {
		boost::throw_exception(std::runtime_error("Can not register window class"));
	}
}

LRESULT CALLBACK Visual::win_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Visual *self = (Visual*)::GetProp(hWnd,TEXT("SELF"));
    if (self) {
       return self->handleMessage(msg, wParam, lParam);
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Visual::handleMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	bool handled = false;
	switch(message) {
	case WM_DESTROY:
		DestroyVisualEvent ev(this);
		handled = handleEvent(&ev);
		break;
	}
	return handled ? TRUE : ::DefWindowProc(hWnd_, message, wParam, lParam);
}

void Visual::show_system_error_dlg(const _TCHAR* message) {
	::MessageBoxEx(NULL,message,TEXT("System error"), MB_OK | MB_ICONERROR, MB_APPLMODAL);
}

void Visual::setVisible(bool visibility) {
	::ShowWindow(hWnd_,visibility ? SW_SHOW : SW_HIDE);
}

bool Visual::isVisible() const {
	return ::IsWindowVisible(hWnd_);
}

Visual::~Visual()
{

}

} // namesapce gui
