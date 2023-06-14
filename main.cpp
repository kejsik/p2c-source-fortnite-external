#include <Windows.h>
#include <string>
#include "string.hpp"
#include <thread>
#include <TlHelp32.h>
#include "World/game.h"
#include "Includes/sdk.hpp"
#include <iostream>
#include "Xor/xor.hpp"

DWORD_PTR ProcessId;
#pragma warning(suppress : 4996).
int process_id;

DWORD FindProcessID(string name) {
    PROCESSENTRY32 ProcessEntry;
    ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &ProcessEntry) == (int)1) {
        while (Process32Next(snapshot, &ProcessEntry) == (int)1) {
            if (string(ProcessEntry.szExeFile) == name)
                return ProcessEntry.th32ProcessID;
        }
    }

    CloseHandle(snapshot);
    return NULL;
}

HWND windowid = NULL;
int main()
{
    ProcessId = FindProcessID("FortniteClient-Win64-Shipping.exe");
    if (!ProcessId) {
        std::cout << ("Couldnt Find game !\nn") << endl;
        Sleep(2000);
        return 0;
    }

    // getting fortnite pid for those who doesnt know xd

        printf("Thrilla Company\n\n ", 50);
        printf("Looking For Fortnite..\n", 50);
        system("cls");
        Sleep(2000);
        printf("Initializing Cheat\n", 50);
        Sleep(2000);;
        printf("Hooked!", 50);
        system("cls");

        //initalize ur driver here

        int injection = MessageBoxA(0, ("Menu key is Insert"), ("Thrilla :)"), MB_OK);
        switch (injection)
        {
            Sleep(100);
        }
     
      ::ShowWindow(::GetConsoleWindow(), SW_HIDE); //hides cheat to task manager
 
      //do ur drivers base address getting and then change it for example from base_address to baseaddy



        cout << baseaddy;
        ShowWindow(::GetConsoleWindow(), SW_HIDE);
        system("cls");
        // neura loaded
        system("Color 02");
    
        settings::width = GetSystemMetrics(SM_CXSCREEN);
        settings::height = GetSystemMetrics(SM_CYSCREEN);

        float ScreenCenterX = settings::width / 2;
        float  ScreenCenterY = settings::height / 2;

        create_overlay(); //overlay loop
        directx_init();
        render_loop(); 
        
        return 0;
}