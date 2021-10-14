#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include<locale>

using namespace std;

unsigned short dat[2];

typedef struct MyStruct
{
    int d1;
    int d2;
    int dt[3];
} GT;

GT gt = { 1,2,{3,4,5} };


// Declare functions.
VOID SwitchFunc(int num);

int main()
{
    /*const char *str1 = "这个图片";
    cout << *str1 << endl;*/
    
    char str2[] = "图片";
    cout << str2 << endl;

    CHAR str3[] = "图片";
    CHAR str31[] = "tp";

    WCHAR str4[] = L"图片";
    WCHAR str41[] = L"tp";

    string str5 = "图片";
    wstring str51 = L"图片";

    const CHAR *p1;
    const WCHAR *p2;
    p1= str5.c_str();
    p2= str51.c_str();

    string str6 = "ts";
    wstring str61 = L"ts";

    wstring filePath = L"C:\\ProgramData\\test.log";

    std::wofstream wf;
    wf.open(filePath, wofstream::app | wofstream::out);
    //wf.imbue(std::locale("chs"));       // 1. 打开文件后需要加上该行，否则无法向文件中写入中文。

    //wf << L"This 图片路径  XX 测试" << L"\n";

    wf << "UTF8 Test 图片中文数据 vvvv.\n" << endl;
    wf.close();

    int inputNum = 0;
    // 如果输入不是int类型将会跳出循环。
    while (cin >> inputNum)
    {
        SwitchFunc(inputNum);
    }
    
   


    system("pause");
}

VOID SwitchFunc(int num)
{
    //必须要将该变量定义成static，否则每次都会被初始化成FALSE,导致前一次的设置无法对后一次生效
    static BOOL status = FALSE; 
    
    switch (num)
    {
    case 1:
        status = FALSE;
        break;
    case 2:
        status = FALSE;
        break;
    case 3:
        status = TRUE;
        break;
    case 4:
        if (status)
        {
            cout << "status is true" << endl;
        }
        break;
    default:
        break;
    }

}