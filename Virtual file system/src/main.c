#include <stdio.h>
#include <stdlib.h>
#include "directory.h"
#include "commandParser.h"

unsigned char *virtualDisk;
FreeBlock *freeHead = NULL;
FreeBlock *freeTail = NULL;
int numBlocks = MAX_BLOCKS;

void initializeVFS() {
    virtualDisk = (unsigned char *)malloc(numBlocks * BLOCK_SIZE);
    for (int i = 0; i < numBlocks; i++)
        insertFreeBlockAtTail(i);

    FileNode *root = (FileNode *)malloc(sizeof(FileNode));
    strcpy(root->name, "/");
    root->isDirectory = 1;
    root->firstChild = NULL;
    root->parent = NULL;
    root->next = root->prev = root;

    runCommandLoop(root);
}

int main(int argc, char *argv[]) {
    if (argc > 1)
        numBlocks = atoi(argv[1]);

    initializeVFS();
    free(virtualDisk);
    return 0;
}

