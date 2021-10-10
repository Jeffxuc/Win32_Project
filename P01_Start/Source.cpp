#include <windows.h>
#include <wchar.h>

// Windows程序的入口点，相当于C中的 main 函数
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    //MessageBox(NULL, TEXT("Hello,windows! 测试数据"), TEXT("This is win32"), 0);

    MessageBox(NULL, TEXT("Test..."), TEXT("Win32"), MB_OK);

    const wchar_t *p1 = L"Hello";

    
    


    return 0;
}