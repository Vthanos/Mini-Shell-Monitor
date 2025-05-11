#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "monitor.h"

void execute_command(char *cmd) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "CreateProcess failed (%d)\n", GetLastError());
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    monitor_process(pi.dwProcessId);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
