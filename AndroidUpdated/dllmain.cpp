/*
This source is free on githuh: https://github.com/Android1337/Basic-Fortnite-Cheat-Source-Internal
If you sell this I will make you famous (in a bad way)
*/

#include "include.h"
#include <Windows.h>


void AllocateConsole() {
    AllocConsole();
    static_cast<VOID>(freopen(xorstr("CONIN$"), xorstr("r"), stdin));
    static_cast<VOID>(freopen(xorstr("CONOUT$"), xorstr("w"), stdout));
    static_cast<VOID>(freopen(xorstr("CONOUT$"), xorstr("w"), stderr));
}


void Initialize() {
    //Here we initialize MinHook
    MH_Initialize();

    //Here we initialize the base Settings
    InitSettings::InitializeSettings();

    //Here we initialize the worker
    if (!worker::Start()) {
        return;
    }

    //Here we initialize the Main Functions
    if (!AndroidUpdated::Start()) {
        return;
    }
}


BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        //AllocateConsole();
        Initialize();
    }

    return TRUE;
}