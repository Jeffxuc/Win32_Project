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

int main()
{
    /*const char *str1 = "���ͼƬ";
    cout << *str1 << endl;*/
    
    char str2[] = "ͼƬ";
    cout << str2 << endl;

    CHAR str3[] = "ͼƬ";
    CHAR str31[] = "tp";

    WCHAR str4[] = L"ͼƬ";
    WCHAR str41[] = L"tp";

    string str5 = "ͼƬ";
    wstring str51 = L"ͼƬ";

    const CHAR *p1;
    const WCHAR *p2;
    p1= str5.c_str();
    p2= str51.c_str();

    string str6 = "ts";
    wstring str61 = L"ts";

    wstring filePath = L"C:\\ProgramData\\test.log";

    std::wofstream wf;
    wf.open(filePath, wofstream::app | wofstream::out);
    //wf.imbue(std::locale("chs"));       // 1. ���ļ�����Ҫ���ϸ��У������޷����ļ���д�����ġ�

    //wf << L"This ͼƬ·��  XX ����" << L"\n";

    wf << "UTF8 Test ͼƬ�������� vvvv.\n" << endl;
    wf.close();




    system("pause");
}