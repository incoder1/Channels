#ifndef __GUI_WINDOWS_VISUAL_HPP_INCLUDED__
#define __GUI_WINDOWS_VISUAL_HPP_INCLUDED__

#include <windows.h>
#include <tchar.h>

#include <stdexcept>
#include <boost/exception/exception.hpp>
#include <boost/thread/once.hpp>

#include <abstractvisual.hpp>

namespace gui {

class Visual:public AbstractVisual {
private:
	static void show_system_error_dlg(const _TCHAR* message);
	static void init_basic_window_class(HINSTANCE hInstance);
	LRESULT handleMessage(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	static LRESULT CALLBACK win_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	Visual(Visual* const parent,
			const bounds& bounds,
			HINSTANCE hInstance,
			const _TCHAR* windowClass,
			DWORD dwStyle,
			DWORD dwExStyle);
	virtual void setVisible(bool visibility);
	virtual bool isVisible() const;
	virtual ~Visual();
public:
	static const _TCHAR* BASIC_VISUAL_WINDOW_CLASS_NAME;
	HWND hWnd_;
};

class DestroyVisualEvent:public WidgetEvent {
public:
	explicit DestroyVisualEvent(Widget* const source):
		WidgetEvent(WM_DESTROY,source)
	{}
};

} // namespace gui

#endif // __GUI_WINDOWS_VISUAL_HPP_INCLUDED__
