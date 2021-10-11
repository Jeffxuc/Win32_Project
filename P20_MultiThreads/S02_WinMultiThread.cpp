#include "S02_WinMultiThread.h"

#pragma comment (lib, "winmm.lib") //Used for "PlaySound" function.
using namespace std;

/***
1. ��������֮ǰ��Ҫ��ע�ᴰ���࣬��������Ҫ���ڹ�������������Ϣ��
2. ���ڹ��������ڴ����ݸ����ڵ���Ϣ�����������û�����ʱ��Windows��Ӧ�ó�����һ����Ϣ��
   ��:Windows�����˸ô��ڹ��̺������ú����Ĳ���������Windows���ͣ��Ҹó�����ܵ��ض���Ϣ��
***/


// ���ڹ��̺���������
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
    hwnd = CreateWindow(szAPPName,                   // ����������
        TEXT("The hello program"),                   // ���ڱ���
        WS_OVERLAPPEDWINDOW,                         // ���ڷ����ʽ
        CW_USEDEFAULT,                               // ��ʼ x ����
        CW_USEDEFAULT,                               // ��ʼ y ����
        CW_USEDEFAULT,                               // ��ʼ x ����ߴ�
        CW_USEDEFAULT,                               // ��ʼ y ����ߴ�
        NULL,                                        // �����ھ��
        NULL,                                        // ���ڲ˵����
        hInstance,                                   // ����ʵ�����
        NULL);                                       // ��������


    // Create buttons for function test...
    HWND hwndBtn1 = CreateWindowEx(0, L"BUTTON", L"Start", WS_VISIBLE | WS_CHILD,
        100, 50, 100, 40, hwnd, (HMENU)IDStartMonitor, NULL, NULL);

    HWND hwndBtn2 = CreateWindowEx(0, L"BUTTON", L"Stop", WS_VISIBLE | WS_CHILD,
        300, 50, 100, 40, hwnd, (HMENU)IDStopMonitor, NULL, NULL);


    ShowWindow(hwnd, nShowCmd);   // CreateWindowֻ�Ǵ���window������ʱ��δ��ʾ����Ļ�ϣ���Ҫ����ShowWindow
    UpdateWindow(hwnd);           // �����ػ洰�ڿͻ�������


    //��Ϣѭ�������������û�����/����
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return msg.wParam;

}


// ���ڹ��̺�����ʵ�֣������˿ͻ�������ʾ���ݣ���������ζ��û�������������Ӧ��
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

