#include <winsock2.h>

#include "gui/Application.h"
#include "gui/WndProc.h"

#include <Windows.h>

#include <string>

#pragma comment(lib, "comctl32.lib")

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    srand(unsigned(time(NULL)));
    if (Application::IsAlreadyRunning())
    {
        return EXIT_FAILURE;
    }

    //INITCOMMONCONTROLSEX icex;
    //icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    //icex.dwICC = ICC_USEREX_CLASSES;
    //InitCommonControlsEx(&icex);

    Application app(NULL, hInstance, nShowCmd, WndProc, 300, 400, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS);
    return app.Run();
}
