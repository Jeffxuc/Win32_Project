#include "S4_MultiTaskEventTriger.h"

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
    HWND    hwnd;
    BOOL    bContinue;           // 用来终止线程的 flag.
    string  curMethod;
} PARAMS, *PPARAMS;

int   curRunningState = -1;    // 判断当前线程运行状态：-1 unknown, 0 stop, 1 running.

// Some function declarations.
LRESULT APIENTRY WndProc(HWND, UINT, WPARAM, LPARAM);
VOID MyThread01(PVOID pvoid);


VOID WriteLog(const std::string& contentStr)
{
    // TODO: need add Check whether the file exists.
    if(!ofstr.is_open())
        ofstr.open(logFile, std::ofstream::app | std::ofstream::out);
    ofstr << contentStr << "\n";

    //ofstr.close();
}

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


    // Create some buttons for function test...
    CreateButonTest(hwnd);

    // Generate Test Data:
    for (int i = 100; i < 161; ++i)
    {
        std::string curStr = "Test log data: " + std::to_string(i);
        testStrData.push_back(curStr);
    }

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    while (GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;

}

VOID CreateButonTest(HWND hwnd)
{
    // Test Multi-Thread method 01.
    HWND hwndBtn11 = CreateWindowEx(0, L"BUTTON", L"Theme01", WS_VISIBLE | WS_CHILD,
        100, 50, 100, 40, hwnd, (HMENU)M01_CASE01, NULL, NULL);

    HWND hwndBtn12 = CreateWindowEx(0, L"BUTTON", L"Theme02", WS_VISIBLE | WS_CHILD,
        300, 50, 100, 40, hwnd, (HMENU)M01_CASE02, NULL, NULL);

    HWND hwndBtn13 = CreateWindowEx(0, L"BUTTON", L"Theme03", WS_VISIBLE | WS_CHILD,
        500, 50, 100, 40, hwnd, (HMENU)M01_CASE03, NULL, NULL);

    // Test Thread Method 02.
    HWND hwndBtn21 = CreateWindowEx(0, L"BUTTON", L"Method2_01", WS_VISIBLE | WS_CHILD,
        100, 140, 100, 40, hwnd, (HMENU)M02_CASE01, NULL, NULL);

    HWND hwndBtn22 = CreateWindowEx(0, L"BUTTON", L"Method2_02", WS_VISIBLE | WS_CHILD,
        300, 140, 100, 40, hwnd, (HMENU)M02_CASE02, NULL, NULL);

    HWND hwndBtn23 = CreateWindowEx(0, L"BUTTON", L"Method2_03", WS_VISIBLE | WS_CHILD,
        500, 140, 100, 40, hwnd, (HMENU)M02_CASE03, NULL, NULL);

    // Test "_beginthreadex()" method control thread.
    HWND hwndBtn31 = CreateWindowEx(0, L"BUTTON", L"Method3_01", WS_VISIBLE | WS_CHILD,
        100, 230, 100, 40, hwnd, (HMENU)M03_CASE01, NULL, NULL);

    HWND hwndBtn32 = CreateWindowEx(0, L"BUTTON", L"Method3_02", WS_VISIBLE | WS_CHILD,
        300, 230, 100, 40, hwnd, (HMENU)M03_CASE02, NULL, NULL);

    HWND hwndBtn33 = CreateWindowEx(0, L"BUTTON", L"Method3_03", WS_VISIBLE | WS_CHILD,
        500, 230, 100, 40, hwnd, (HMENU)M03_CASE03, NULL, NULL);

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
        int countNum = wsprintf(szBuffer, szMessage[iStatus], REP, lTime);
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
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        switch (wmId)
        {
        case M01_CASE01:
        {
            params.bContinue = FALSE;
            params.curMethod = "_M01";

            _beginthread(MyThread02, 0, &params);
            return 0;
        }
        case M01_CASE02:
        {
            params.bContinue = FALSE;    
            params.curMethod = "_M02";

            _beginthread(MyThread02, 0, &params);

            return 0;
        }
        case M01_CASE03:
        {
            
            return 0;
        }
        case M02_CASE01:
        {
            // Terminate thread.
            if (hThread != NULL)
            {
                WaitForSingleObject(hThread, 10);
                CloseHandle(hThread);
            }

            params.bContinue = TRUE;
            params.curMethod = "M02_Case01";

            hThread = CreateThread(
                NULL,
                0,
                MyThreadFunction_01,
                &params,
                0,
                &dwThreadId);

            //Check the return value for success. If CreateThread fails, terminate execution.
            if (hThread == NULL)
            {
                ExitProcess(3);
            }

            WaitForSingleObject(hThread, 10);
            CloseHandle(hThread);

            return 0;
        }
        case M02_CASE02:
        {
            // Terminate thread.
            if (hThread != NULL)
            {
                WaitForSingleObject(hThread, 10);
                CloseHandle(hThread);
            }

            params.bContinue = TRUE;
            params.curMethod = "M02_Case02";

            hThread = CreateThread(
                NULL,
                0,
                MyThreadFunction_01,
                &params,
                0,
                &dwThreadId);

            //Check the return value for success. If CreateThread fails, terminate execution.
            if (hThread == NULL)
            {
                ExitProcess(3);
            }

            WaitForSingleObject(hThread, 10);
            CloseHandle(hThread);

            return 0;


        }
        case M02_CASE03:
        {
            return 0;
        }
        case M03_CASE01:
        {
            WriteLog("Current method 03_01.");

            params.bContinue = TRUE;
            params.curMethod = "_Case01";

            HANDLE hThread_01 = (HANDLE)_beginthreadex(
                NULL,
                0,
                MyThreadFunction_02,
                &params,
                0,
                &threadId);

            WaitForSingleObject(hThread_01, INFINITE);
            CloseHandle(hThread_01);
            //params.bContinue = FALSE;

            WriteLog("M03_01 thread terminate.");
            return 0;
        }
        case M03_CASE02:
        {
            params.bContinue = FALSE;
            
            WriteLog("Current method 03_02.");
            params.curMethod = "_Case02";
            params.bContinue = TRUE;

            /*HANDLE hThread_02 = (HANDLE)_beginthreadex(
                NULL,
                0,
                MyThreadFunction_02,
                &params,
                0,
                &threadId);

            WaitForSingleObject(hThread_02, INFINITE);
            CloseHandle(hThread_02);*/


            return 0;
        }
        case M03_CASE03:
        {

            return 0;
        }
        default:
            return 0;
        }

    }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}


VOID MyThread01(PVOID pvoid)
{
    double a = 1.0;
    INT  i;
    LONG lTime;
    volatile PPARAMS pParams;  //因为会在外部进行修改，所以要避免被编译器优化。
    pParams = (PPARAMS)pvoid;

    lTime = GetCurrentTime();

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


VOID MyThread02(PVOID pvoid)
{
    while (true)
    {
        if(curRunningState==-1 || curRunningState==0)
            break;
    }

    volatile PPARAMS pParams;
    pParams = (PPARAMS)pvoid;

    pParams->bContinue = TRUE;


    while (TRUE)
    {
        curRunningState = 1;

        for (vector<string>::iterator it = testStrData.begin(); it != testStrData.end(); it++)
        {
            if(!pParams->bContinue)
                break;

            WriteLog(*it + pParams->curMethod);
            Sleep(20);
        }

        if (pParams->bContinue == TRUE)
        {
            WriteLog("====End a loop....");
        }
        else
        {
            break;
        }
          
    }

    curRunningState = 0;
    WriteLog("*******Terminate the current thread.************");

    _endthread();

}

DWORD WINAPI MyThreadFunction_01(LPVOID pvoid)
{
    volatile PPARAMS pParams;
    pParams = (PPARAMS)pvoid;

    while (pParams->bContinue)
    {
        for (vector<string>::iterator it = testStrData.begin(); it != testStrData.end() && pParams->bContinue; it++)
        {
            *it += pParams->curMethod;
            WriteLog(*it);
            Sleep(10);
        }
    }

    return 0;
}

// Used for "_beginthreadex()" method.
unsigned WINAPI MyThreadFunction_02(PVOID pvoid)
{
    volatile PPARAMS pParams;
    pParams = (PPARAMS)pvoid;

    while (pParams->bContinue)
    {
        for (vector<string>::iterator it = testStrData.begin(); it != testStrData.end() && pParams->bContinue; it++)
        {
            WriteLog(*it + pParams->curMethod);

            Sleep(30);
        }

        WriteLog("******** A loop vector string final.*************");
    }

    std::string stateStr = pParams->bContinue == TRUE ? "bContinue = TRUE" : "bContinue = FALSE";
    WriteLog(stateStr);

    _endthreadex(0);

    return 0;
}








