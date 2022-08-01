#include <windows.h>
#include <stdio.h>

#define  FG

#ifdef _DEBUG
#define  Ps "Hello"
#else
#define Ps "Now release mode"
#endif 

#ifdef FG
#define  a a+1
#define  ver _MSC_VER
#endif



int main()
{
    char *p1 = "Hello";
    wchar_t *p2 = L"Hello";

    char p5[] = "hello";

    char* pStr1 = "hello world";
    char pStr2[] = "hello world";

    signed char* pStr3 = "hello";
    unsigned char* pStr4 = "hello";

    char* s1 = "hello world";
    signed char* s2 = "hello world";
    unsigned char* s3 = "hello world";


    char *p3;
    p3 = &Ps;
    int v = ver;



    strlen(p1);

    printf("the num is %i and %i \n", 10, 50);

    system("pause");
}