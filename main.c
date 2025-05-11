#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exec_utils.h"

int main() {
    char command[256];
    while (1) {
        printf("mini-shell> ");
        if (fgets(command, sizeof(command), stdin) == NULL) break;
        command[strcspn(command, "\n")] = 0;
        if (strcmp(command, "exit") == 0) break;
        if (strlen(command) > 0) execute_command(command);
    }
    return 0;
}
