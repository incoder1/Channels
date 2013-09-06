#ifndef WINDOWS_CONSOLE_CONTROL_HPP_INCLUDED
#define WINDOWS_CONSOLE_CONTROL_HPP_INCLUDED

#include <iostream>
#include <Windows.h>
#include <Richedit.h>

#if !defined MSFTEDIT_CLASS
#define MSFTEDIT_CLASS L"RICHEDIT50W"
#endif

namespace gui {

class WindowsConsoleControl
{
	public:
		WindowsConsoleControl();
		void inject(HWND parent) const;
		void redirectStdOut() const;
		void resize(DWORD width, DWORD heigth) const;
		~WindowsConsoleControl();
	private:
		mutable HWND richEdit_;
		HMODULE module_;
};

} // namespace gui

#endif // WINDOWS_CONSOLE_CONTROL_HPP_INCLUDED
