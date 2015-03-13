#include <windows.h>

#include "visual.hpp"

#include <boost/bind.hpp>

class Frame:public gui::Visual {
public:

public:
	Frame(HINSTANCE hInstance):
		Visual(NULL,gui::create_bounds(0,0,640,480),hInstance,NULL,WS_OVERLAPPEDWINDOW,WS_EX_OVERLAPPEDWINDOW)
	{
		connect(WM_DESTROY, boost::bind(&Frame::onDestroy,this,_1) );
	}
	bool onDestroy(const gui::WidgetEvent* ev) {
		::PostQuitMessage(0);
		return true;
	}
};


int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
	Frame *frame = new Frame(hThisInstance);
	frame->setVisible(true);

	MSG messages;            /* Here messages to the application are saved */
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


