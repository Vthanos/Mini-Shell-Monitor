#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

void monitor_process(pid_t pid) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("Unable to open status file");
        return;
    }
    printf("\n[Process Resource Usage]\n");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "VmRSS") || strstr(line, "voluntary_ctxt_switches")) {
            printf("%s", line);
        }
    }
    fclose(fp);
}
