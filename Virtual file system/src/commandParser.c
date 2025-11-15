#include <stdio.h>
#include <string.h>
#include "commandParser.h"

void runCommandLoop(FileNode *rootDirectory) {
    FileNode *currentDir = rootDirectory;
    char command[256], arg1[100], arg2[512];

    printf("Compact VFS ready. Type 'exit' to quit.\n");

    while (1) {
        printf("%s > ", currentDir->name);
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (strncmp(command, "mkdir ", 6) == 0)
            makeDirectory(currentDir, command + 6);
        else if (strncmp(command, "create ", 7) == 0)
            createFile(currentDir, command + 7);
        else if (strncmp(command, "write ", 6) == 0) {
            sscanf(command + 6, "%s \"%[^\"]\"", arg1, arg2);
            writeFile(currentDir, arg1, arg2);
        }
        else if (strncmp(command, "read ", 5) == 0)
            readFile(currentDir, command + 5);
        else if (strncmp(command, "delete ", 7) == 0)
            deleteFile(currentDir, command + 7);
        else if (strncmp(command, "rmdir ", 6) == 0)
            removeDirectory(currentDir, command + 6);
        else if (strcmp(command, "ls") == 0)
            listDirectory(currentDir);
        else if (strncmp(command, "cd ", 3) == 0)
            changeDirectory(&currentDir, command + 3);
        else if (strcmp(command, "pwd") == 0) {
            printWorkingDirectory(currentDir);
            printf("\n");
        }
        else if (strcmp(command, "exit") == 0)
            break;
        else
            printf("Unknown command\n");
    }
}
