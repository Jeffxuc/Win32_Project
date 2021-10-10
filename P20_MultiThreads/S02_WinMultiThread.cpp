#include "S02_WinMultiThread.h"
#include <windows.h>

#pragma comment (lib, "winmm.lib") //Used for "PlaySound" function.

/***
1. ��������֮ǰ��Ҫ��ע�ᴰ���࣬��������Ҫ���ڹ�������������Ϣ��
2. ���ڹ��������ڴ����ݸ����ڵ���Ϣ�����������û�����ʱ��Windows��Ӧ�ó�����һ����Ϣ��
   ��:Windows�����˸ô��ڹ��̺������ú����Ĳ���������Windows���ͣ��Ҹó�����ܵ��ض���Ϣ��
***/


// ���ڹ��̺���������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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

    if (!RegisterClass(&wndClass))
    {
        MessageBox(NULL, TEXT("This program requires windows NT!"), szAPPName, MB_ICONERROR);
        return 0;

    }

    hwnd = CreateWindow(szAPPName,                   // ����������
        TEXT("The hello program"),   // ���ڱ���
        WS_OVERLAPPEDWINDOW,         // ���ڷ����ʽ
        CW_USEDEFAULT,               // ��ʼ x ����
        CW_USEDEFAULT,               // ��ʼ y ����
        CW_USEDEFAULT,               // ��ʼ x ����ߴ�
        CW_USEDEFAULT,               // ��ʼ y ����ߴ�
        NULL,                        // �����ھ��
        NULL,                        // ���ڲ˵����
        hInstance,                   // ����ʵ�����
        NULL);                       // ��������

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
        GetClientRect(hwnd, &rect);
        DrawText(hdc, TEXT("My test for win32!"), -1, &rect,
            DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hwnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    }

    return DefWindowProc(hwnd, message, wParam, lParam);

}
