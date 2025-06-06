#include "CrashHandler.hpp"

#include <Windows.h>
// Windows.h must be before
#include <DbgHelp.h>
#include <iostream>

#pragma comment(lib, "dbghelp.lib")

LONG WINAPI CrashHandler(EXCEPTION_POINTERS* exceptionInfo)
{
    void* stack[64];
    HANDLE const process = GetCurrentProcess();
    SymInitialize(process, nullptr, TRUE);
    USHORT const frames = CaptureStackBackTrace(0, 64, stack, nullptr);

    std::cerr << "=== Stack Trace ===\n";
    for (USHORT i = 0; i < frames; ++i)
    {
        DWORD64 const address = reinterpret_cast<DWORD64>(stack[i]);
        char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME];
        auto const symbol = reinterpret_cast<PSYMBOL_INFO>(buffer);
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYM_NAME;

        DWORD64 displacement;
        if (SymFromAddr(process, address, &displacement, symbol))
        {
            std::cerr << i << ": " << symbol->Name << " - 0x" << std::hex << symbol->Address << std::dec << "\n";
        }
        else
        {
            std::cerr << i << ": [0x" << std::hex << address << "] (no symbol)\n";
        }
    }

    return EXCEPTION_EXECUTE_HANDLER;
}

void InstallCrashHandler()
{
    SetUnhandledExceptionFilter(CrashHandler);
}
