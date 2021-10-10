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

    char *p3;
    p3 = &Ps;
    int v = ver;



    strlen(p1);

    printf("the num is %i and %i \n", 10, 50);

    system("pause");
}