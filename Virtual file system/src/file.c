#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "file.h"

extern unsigned char *virtualDisk;
extern FreeBlock *freeHead, *freeTail;
extern int numBlocks;

void createFile(FileNode *currentDir, const char *name) {
    if (findChild(currentDir, name)) {
        printf("File already exists\n");
        return;
    }
    FileNode *file = (FileNode *)malloc(sizeof(FileNode));
    strcpy(file->name, name);
    file->isDirectory = 0;
    file->blockPointers = NULL;
    file->blocksAllocated = 0;
    file->contentSize = 0;
    insertChild(currentDir, file);
    printf("File '%s' created.\n", name);
}

void writeFile(FileNode *currentDir, const char *name, const char *data) {
    FileNode *file = findChild(currentDir, name);
    if (!file || file->isDirectory) {
        printf("File not found\n");
        return;
    }
    if (file->blockPointers) {
        for (int i = 0; i < file->blocksAllocated; i++)
            freeBlock(file->blockPointers[i]);
        free(file->blockPointers);
        file->blockPointers = NULL;
        file->blocksAllocated = 0;
    }

    int len = strlen(data);
    int blocksNeeded = (int)ceil((double)len / BLOCK_SIZE);
    file->blockPointers = (int *)malloc(blocksNeeded * sizeof(int));

    for (int i = 0; i < blocksNeeded; i++) {
        int block = allocateBlock();
        if (block == -1) {
            printf("No free blocks available\n");
            return;
        }
        file->blockPointers[i] = block;
        memcpy(virtualDisk + block * BLOCK_SIZE, data + i * BLOCK_SIZE, BLOCK_SIZE);
    }
    file->blocksAllocated = blocksNeeded;
    file->contentSize = len;
    printf("Written %d bytes to '%s'\n", len, name);
}

void readFile(FileNode *currentDir, const char *name) {
    FileNode *file = findChild(currentDir, name);
    if (!file || file->isDirectory) {
        printf("File not found\n");
        return;
    }

    char *buf = (char *)malloc(file->contentSize + 1);
    int copied = 0;
    for (int i = 0; i < file->blocksAllocated; i++) {
        int bytesToCopy = (file->contentSize - copied > BLOCK_SIZE) ? BLOCK_SIZE : file->contentSize - copied;
        memcpy(buf + copied, virtualDisk + file->blockPointers[i] * BLOCK_SIZE, bytesToCopy);
        copied += bytesToCopy;
    }

    buf[file->contentSize] = '\0';
    printf("%s\n", buf);
    free(buf);
}

void deleteFile(FileNode *currentDir, const char *name) {
    FileNode *file = findChild(currentDir, name);
    if (!file || file->isDirectory) {
        printf("File not found\n");
        return;
    }

    for (int i = 0; i < file->blocksAllocated; i++)
        freeBlock(file->blockPointers[i]);

    free(file->blockPointers);
    removeChild(currentDir, file);
    free(file);
    printf("File '%s' deleted.\n", name);
}
