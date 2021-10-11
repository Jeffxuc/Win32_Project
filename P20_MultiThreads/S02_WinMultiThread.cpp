#include "S02_WinMultiThread.h"

#pragma comment (lib, "winmm.lib") //Used for "PlaySound" function.
using namespace std;

/***
1. 创建窗口之前需要先注册窗口类，窗口类需要窗口过程来处理窗口消息。
2. 窗口过程是用于处理传递给窗口的消息函数，当有用户操作时，Windows向应用程序发送一条消息，
   即:Windows调用了该窗口过程函数，该函数的参数描述了Windows发送，且该程序接受的特定消息。
***/


// 窗口过程函数的声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

VOID MyThread_1(PVOID pvoid);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    static TCHAR szAPPName[] = TEXT("Hello Win");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndClass;

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = szAPPName;

    // Check if the wndClass register. 
    if (!RegisterClass(&wndClass))
    {
        MessageBox(NULL, TEXT("This program requires windows NT!"), szAPPName, MB_ICONERROR);
        return 0;

    }

    // Create the window and some controls/content for it.

    // Create main window for win32 app.
    hwnd = CreateWindow(szAPPName,                   // 窗口类名称
        TEXT("The hello program"),                   // 窗口标题
        WS_OVERLAPPEDWINDOW,                         // 窗口风格或格式
        CW_USEDEFAULT,                               // 初始 x 坐标
        CW_USEDEFAULT,                               // 初始 y 坐标
        CW_USEDEFAULT,                               // 初始 x 方向尺寸
        CW_USEDEFAULT,                               // 初始 y 方向尺寸
        NULL,                                        // 父窗口句柄
        NULL,                                        // 窗口菜单句柄
        hInstance,                                   // 程序实例句柄
        NULL);                                       // 创建参数


    // Create buttons for function test...
    HWND hwndBtn1 = CreateWindowEx(0, L"BUTTON", L"Start", WS_VISIBLE | WS_CHILD,
        100, 50, 100, 40, hwnd, (HMENU)IDStartMonitor, NULL, NULL);

    HWND hwndBtn2 = CreateWindowEx(0, L"BUTTON", L"Stop", WS_VISIBLE | WS_CHILD,
        300, 50, 100, 40, hwnd, (HMENU)IDStopMonitor, NULL, NULL);


    ShowWindow(hwnd, nShowCmd);   // CreateWindow只是创建window，但此时并未显示在屏幕上，需要调用ShowWindow
    UpdateWindow(hwnd);           // 更新重绘窗口客户区内容


    //消息循环，用来处理用户输入/操作
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return msg.wParam;

}


// 窗口过程函数的实现：决定了客户区的显示内容，及窗口如何对用户的输入做出响应。
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    switch (message)
    {
    case WM_CREATE:
        PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        // TODO: the paint code
        GetClientRect(hwnd, &rect);
        DrawText(hdc, TEXT("My test for win32 programming!"), -1, &rect,
            DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        EndPaint(hwnd, &ps);
        return 0;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDStartMonitor:
        {
            g_isRun = TRUE;
            _beginthread(MyThread_1, 0, NULL);
            return 0;
        }
        case IDStopMonitor:
        {
            g_isRun = FALSE;
            return 0;
        }
        default:
            return 0;
        }
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    }

    return DefWindowProc(hwnd, message, wParam, lParam);

}

VOID MyThread_1(PVOID pvoid)
{
    string filePath = "D:\\Temp\\SystemNotification\\muiltiTask.txt";
    ofstream wf;
    wf.open(filePath, ofstream::trunc | ofstream::out);
    int i = 0;

    while (g_isRun && wf.is_open())
    {
        wf << i << " && ";
        ++i;
        Sleep(100);
    }

    wf.close();

    _endthread();
}

