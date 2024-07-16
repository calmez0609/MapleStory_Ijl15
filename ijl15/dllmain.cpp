// dllmain.cpp : 定義 DLL 應用程式的進入點。
#include "stdafx.h"
#include "MsCRC.h"
#include "MultiMs.h"
#include "MsPatch.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <fwpmu.h>
#include "detours.h"  // 引入 Detours 库

#pragma comment(lib, "fwpuclnt.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "detours.lib")  // 链接 Detours 库

typedef struct hostent* (WINAPI* GETHOSTBYNAME)(const char* name);
GETHOSTBYNAME original_gethostbyname;

struct hostent* WINAPI hooked_gethostbyname(const char* name)
{

    if (strcmp(name, "tw.login.maplestory.gamania.com") == 0)
    {
        static struct hostent result;
        static struct in_addr addr;
        static char* addr_list[2];
        static char namebuf[256];

        inet_pton(AF_INET, "127.0.0.1", &addr);
        addr_list[0] = (char*)&addr;
        addr_list[1] = NULL;

        result.h_name = namebuf;
        strcpy_s(namebuf, sizeof(namebuf), name);
        result.h_aliases = NULL;
        result.h_addrtype = AF_INET;
        result.h_length = sizeof(addr);
        result.h_addr_list = addr_list;

        return &result;
    }

    return original_gethostbyname(name);
}



BOOL HookWinsock()
{
    HMODULE hModule = GetModuleHandle(L"ws2_32.dll");
    if (hModule)
    {

        original_gethostbyname = (GETHOSTBYNAME)GetProcAddress(hModule, "gethostbyname");
        if (original_gethostbyname)
        {


            // 使用 Detours 库来挂钩
            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            DetourAttach(&(PVOID&)original_gethostbyname, hooked_gethostbyname);
            if (DetourTransactionCommit() == NO_ERROR)
            {
                MessageBoxA(NULL, "Hook successful", "DNS Query", MB_OK);
                return TRUE;
            }
            else
            {
                MessageBoxA(NULL, "Hook failed", "DNS Query", MB_OK);
            }
        }
        else
        {
            MessageBoxA(NULL, "GetProcAddress failed", "DNS Query", MB_OK);
        }
    }
    else
    {
        MessageBoxA(NULL, "GetModuleHandle failed", "DNS Query", MB_OK);
    }
    return FALSE;
}

DWORD WINAPI MonitorDNS(LPVOID lpParam)
{
    while (true)
    {
        HookWinsock();  // 确保持续钩住函数
        Sleep(1000);    // 每秒钟尝试钩住一次
    }
    return 0;
}

BOOL WINAPI Injected()
{
    // Waitting for Maplestory.exe load
    Sleep(5000);
    // 多開
    MultiClient();
    // HS Bypass
    MsCrcBypass();
    // Patch function
    No_Cap();
    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, MonitorDNS, NULL, 0, NULL); // 创建持续监控线程
        if (HookWinsock()) {
            MessageBox(NULL, L"OK", L"HookWinsock Status", MB_OK);
            CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(&Injected), NULL, 0, NULL);
        }
        else {
            MessageBox(NULL, L"Fail", L"HookWinsock Status", MB_OK);
        }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}