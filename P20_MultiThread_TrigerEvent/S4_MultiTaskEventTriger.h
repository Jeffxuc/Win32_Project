#pragma once
#include <windows.h>
#include <math.h>
#include <process.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

#define M01_CASE01 1001
#define M01_CASE02 1002
#define M01_CASE03 1003

#define M02_CASE01 2001
#define M02_CASE02 2002
#define M02_CASE03 2003

#define M03_CASE01 3001
#define M03_CASE02 3002
#define M03_CASE03 3003

DWORD      dwThreadId;
HANDLE     hThread;
unsigned   threadId;

using namespace std;

string logFile = "C:\\ProgramData\\MultiThreadTest.log";
std::ofstream ofstr;
std::vector<std::string> testStrData;

VOID CreateButonTest(HWND hwnd);

VOID WriteLog(const std::string& contentStr);

VOID MyThread02(PVOID pvoid);

DWORD WINAPI MyThreadFunction_01(LPVOID pvoid);

unsigned WINAPI MyThreadFunction_02(PVOID pvoid);




