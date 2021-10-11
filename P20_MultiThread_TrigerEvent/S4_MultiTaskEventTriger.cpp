#include <windows.h>
#include <math.h>
#include <process.h>
#include <string.h>

#define REP               30000000
#define STATUS_READY      0
#define STATUS_WORKING    1
#define STATUS_DONE       2

// 计算正常结束
#define WM_CALC_DONE      (WM_USER + 0)  
// 计算被用户终止
#define WM_CALC_ABORTED   (WM_USER + 1)

typedef struct MyStruct
{
    HWND hwnd;
    BOOL bContinue;   //用来终止线程的flag
} PARAMS, *PPARAMS;

// Some function declarations.
LRESULT APIENTRY WndProc(HWND, UINT, WPARAM, LPARAM);
VOID MyThread01(PVOID pvoid);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    static TCHAR  szAppName[] = TEXT("MultiTask in back thread.");
    HWND          hwnd;
    MSG           msg;
    WNDCLASS      wndClass;

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = szAppName;

    // Check if the wndClass register. 
    if (!RegisterClass(&wndClass))
    {
        MessageBox(NULL, TEXT("This program requires windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }

    // Create main window for win32 app.
    hwnd = CreateWindow(szAppName,                   // 窗口类名称
        TEXT("Multi-Thread demo exercise "),         // 窗口标题
        WS_OVERLAPPEDWINDOW,                         // 窗口风格或格式
        CW_USEDEFAULT,                               // 初始 x 坐标
        CW_USEDEFAULT,                               // 初始 y 坐标
        CW_USEDEFAULT,                               // 初始 x 方向尺寸
        CW_USEDEFAULT,                               // 初始 y 方向尺寸
        NULL,                                        // 父窗口句柄
        NULL,                                        // 窗口菜单句柄
        hInstance,                                   // 程序实例句柄
        NULL);                                       // 创建参数


    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    while (GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;

}


LRESULT APIENTRY WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static INT              iStatus;
    static LONG             lTime;
    static PARAMS           params;
    static const TCHAR      *szMessage[] = { TEXT("Ready: left mouse button start"),
                                             TEXT("Working: right mouse button ends"),
                                             TEXT("%d repetitions in %ld msec") };

    HDC             hdc;
    PAINTSTRUCT     ps;
    RECT            rect;
    TCHAR           szBuffer[64];

    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        if (iStatus == STATUS_WORKING)
        {
            //Plays a waveform sound which specified as the Windows Default Beep sound.
            MessageBeep(0); 
            return 0;
        }

        iStatus = STATUS_WORKING;
        params.hwnd = hwnd;
        params.bContinue = TRUE;

        _beginthread(MyThread01, 0, &params);

        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }
    case WM_RBUTTONDOWN:
    {
        params.bContinue = FALSE;
        return 0;
    }
    case WM_CALC_DONE:
    {
        lTime = lParam;
        iStatus = STATUS_DONE;
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }
    case WM_CALC_ABORTED:
    {
        iStatus = STATUS_READY;
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);

        /********** TODO: the customized content for drawing. ************/
        GetClientRect(hwnd, &rect);
        wsprintf(szBuffer, szMessage[iStatus], REP, lTime);
        DrawText(hdc, szBuffer, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        /*****************************************************************/

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}


VOID MyThread01(PVOID pvoid)
{
    double a = 1.0;
    INT  i;
    LONG lTime;
    volatile PPARAMS pParams;  //避免被编译器优化。
    pParams = (PPARAMS)pvoid;

    lTime = GetCurrentTime();  //返回当前时间，单位为毫秒

    for (i = 0; i < REP && pParams->bContinue; ++i)
    {
        a = tan(atan(exp(log(sqrt(a*a))))) + 1.0;
    }

    if (i == REP)
    {
        lTime = GetCurrentTime() - lTime;
        SendMessage(pParams->hwnd, WM_CALC_DONE, 0, lTime);
    }
    else
    {
        SendMessage(pParams->hwnd, WM_CALC_ABORTED, 0, 0);
    }

    _endthread();

}




