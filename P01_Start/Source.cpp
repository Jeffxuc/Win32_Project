#include <windows.h>
#include <wchar.h>

// Windows�������ڵ㣬�൱��C�е� main ����
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    //MessageBox(NULL, TEXT("Hello,windows! ��������"), TEXT("This is win32"), 0);

    MessageBox(NULL, TEXT("Test..."), TEXT("Win32"), MB_OK);

    const wchar_t *p1 = L"Hello";

    
    


    return 0;
}