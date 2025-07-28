#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "monitor.h"

void execute_command(char *cmd) {
    char *commands[128];
    int num_commands = 0;
    char *next_cmd = strtok(cmd, "|");

    // Split the input into individual commands
    while (next_cmd != NULL) {
        commands[num_commands++] = next_cmd;
        next_cmd = strtok(NULL, "|");
    }

    HANDLE hOldRead = NULL;  // Previous read handle for chaining pipes

    for (int i = 0; i < num_commands; ++i) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        ZeroMemory(&pi, sizeof(pi));
        si.cb = sizeof(si);
        si.dwFlags |= STARTF_USESTDHANDLES;

        HANDLE hRead, hWrite;

        // Create a pipe for the current process
        if (i < num_commands - 1) {
            if (!CreatePipe(&hRead, &hWrite, NULL, 0)) {
                fprintf(stderr, "CreatePipe failed (%d)\n", GetLastError());
                return;
            }
            SetHandleInformation(hWrite, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
        } else {
            hRead = NULL;
            hWrite = NULL;
        }

        // Assign the appropriate handles for input/output redirection
        si.hStdInput = (i == 0) ? GetStdHandle(STD_INPUT_HANDLE) : hOldRead;
        si.hStdOutput = (i < num_commands - 1) ? hWrite : GetStdHandle(STD_OUTPUT_HANDLE);
        si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

        // Create the process for the current command
        if (!CreateProcess(NULL, commands[i], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            fprintf(stderr, "CreateProcess failed (%d)\n", GetLastError());
            return;
        }

        // Close unused handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        if (hWrite != NULL) CloseHandle(hWrite);
        if (hOldRead != NULL) CloseHandle(hOldRead);

        // Keep the read handle for the next process
        hOldRead = hRead;
    }

    if (hOldRead != NULL) CloseHandle(hOldRead);
}