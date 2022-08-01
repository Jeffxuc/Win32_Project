#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include<locale>
#include <sstream>
#include <cstring>
#include <cstdlib>


//#define T_002

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
VOID SwitchFunc01(int num);
VOID TestDataType();

/// <summary>
/// Used for converting character from one style to another.
/// </summary>
VOID ConvertCharacter();

// 该方法会造成内存泄露
WCHAR* char2wchar(CHAR* charArr)
{
    size_t newsize = strlen(charArr) + 1;
    WCHAR* wcstring = new WCHAR[newsize];
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, wcstring, newsize, charArr, _TRUNCATE);
    return wcstring;
}


wchar_t* CharToWchar(const char* charSrc)
{
    size_t newsize = strlen(charSrc) + 1;
    wchar_t* wcString = new wchar_t[newsize];
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, wcString, newsize, charSrc, _TRUNCATE);
    
    return wcString;
}

wstring StringToWString(const string& strSrc)
{
    wstring strDest = L"";
    int nSize = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), (int)strSrc.length(), 0, 0);
    if (nSize <= 0)
        return strDest;

    wchar_t* pwszDst = new wchar_t[nSize + 1];
    int iRet = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), (int)strSrc.length(), pwszDst, nSize);
    pwszDst[nSize] = 0;

    strDest.assign(pwszDst);
    delete[] pwszDst;

    return strDest;
}


int main()
{
    ConvertCharacter();
    //TestDataType();

#ifdef T_001

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
    p1 = str5.c_str();
    p2 = str51.c_str();

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

#endif // T_001



#ifdef T_002

    UCHAR* TransferBuffer = new UCHAR[512]{ 8 };
    TransferBuffer[0] = 106;
    TransferBuffer[1] = 107;
    TransferBuffer[2] = 1;
    TransferBuffer[3] = 67;
    TransferBuffer[4] = 16;
    TransferBuffer[5] = 0;

    int res1 = TransferBuffer[2];
    int res2 = TransferBuffer[5];

    char* tsbf = new char[512]{ 0 };
    tsbf = reinterpret_cast<char*>(TransferBuffer);
    std::string resStr(tsbf,5);

    // 当字符值为char时。
    std::string chStr01;
    std::stringstream sstream;
    for (int i = 0; i < 8; ++i)
    {
        sstream << TransferBuffer[i];
    }

    chStr01 = sstream.str();

    UCHAR arr1[64] = { 0xFF,0x01,0x00 };
    int s1 = arr1[4];
    
    UCHAR uch = '0';

    



#endif // T_002

    
   


    //system("pause");
}

VOID SwitchFunc01(int num)
{
    string str = "";
    while (true)
    {
        switch (num)
        {
        case 1:
            str = "case 01";
            break;
        case 2:
            str = "case 02";
            break;
        case 3:
            str = "case 03";
            goto newRes;
        default:
            goto newRes;
        }

    }

newRes:
    int i = 100;
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
            std::cout << "status is true" << endl;
        }
        break;
    default:
        break;
    }

}


VOID TestDataType()
{
    int i = 10;
    char c1 = 's';
    CHAR c2 = 's';
    const wchar_t *wc01 = L"ts";

    const char* pStr1 = "hello world";
    char pStr2[] = "hello world";

    pStr1 = "yes";
    pStr2[1] = 'Y';

    const char *pc1 = "hello";
    char pc2[] = "hello";
    bool flag = false;

    //const unsigned char* pc3 = "Thanks you";

    short s1 = 10;
    long s2 = 10;
    long long s3 = 10;

    if (i)
    {
        i = 0;
        //pc1 = pc2;
        pc2[1] = 'y';
    }

    sizeof(i);

}

VOID Tmp(unsigned char* pRet)
{
    pRet = (unsigned char*)"GetValue Num";
}

VOID Tmp2(unsigned char* pRet)
{
    const char* tmp = "Func-Tmp2";
    size_t sizeNum = strlen(tmp) + 1;
    strcpy_s((char*)pRet, sizeNum, tmp);
}

//char* WcharToChar(const wchar_t* wchSrc)
//{
//    char* m_char;
//    int len = WideCharToMultiByte(CP_ACP, 0, wchSrc, (int)wcslen(wchSrc), NULL, 0, NULL, NULL);
//    m_char = new char[len + 1];
//    WideCharToMultiByte(CP_ACP, 0, wchSrc, (int)wcslen(wchSrc), m_char, len, NULL, NULL);
//    m_char[len] = '\0';
//    return m_char;
//}

char* WcharToChar(const wchar_t* wchSrc)
{
    size_t origSize = wcslen(wchSrc) + 1;
    size_t convertedChars = 0;

    const size_t newSize = origSize * 2;
    char* retStr = new char[newSize];
    wcstombs_s(&convertedChars, retStr, newSize, wchSrc, _TRUNCATE);

    return retStr;
}




VOID ConvertCharacter()
{
    /*const char* p1 = "hello world";
    wchar_t* res01 = CharToWchar(p1);

    
    const char* pSrc = "test";
    char* pDest_1 = (char*)pSrc;

    unsigned char* p3 = (unsigned char*)pDest_1;
    unsigned char* p4 = reinterpret_cast<unsigned char*>(pDest_1);

    char* p5 = (char*)p3;
    char* p6 = reinterpret_cast<char*>(p3);*/

    std::string str("test");
    char* pChar = new char[str.size() + 1];
    std::copy(str.begin(), str.end(), pChar);
    // C-Style字符串要在末尾添加空截至符
    pChar[str.size()] = '\0';

    const wchar_t* pSrc = L"Hello";
    wchar_t* pCom1 = new wchar_t[6]();
    wcscpy_s(pCom1, 6, pSrc);
    delete[] pCom1;

    wchar_t wcStr[100];
    wcscpy_s(wcStr, _countof(wcStr), pSrc);

    
    const char* pChStr = "Test";
    
    
    

    
    

    
    
    





    

    //system("pause");

}