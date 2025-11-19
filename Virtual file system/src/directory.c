#include <stdlib.h>
#include <string.h>
#include "directory.h"

extern unsigned char *virtualDisk;
extern FreeBlock *freeHead, *freeTail;
extern int numBlocks;

void insertFreeBlockAtTail(const int index) {
    FreeBlock *newBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
    newBlock->blockIndex = index;
    newBlock->next = NULL;
    newBlock->prev = freeTail;

    if (!freeHead) freeHead = newBlock;
    if (freeTail) freeTail->next = newBlock;
    freeTail = newBlock;
}

int allocateBlock() {
    if (!freeHead) return -1;

    int idx = freeHead->blockIndex;
    FreeBlock *tmp = freeHead;
    freeHead = freeHead->next;
    if (freeHead) freeHead->prev = NULL;
    else freeTail = NULL;

    free(tmp);
    return idx;
}

void freeBlock(const int index) {
    insertFreeBlockAtTail(index);
}

void insertChild(FileNode *parent, FileNode *child) {
    child->parent = parent;
    if (!parent->firstChild) {
        parent->firstChild = child;
        child->next = child->prev = child;
    } else {
        FileNode *last = parent->firstChild->prev;
        last->next = child;
        child->prev = last;
        child->next = parent->firstChild;
        parent->firstChild->prev = child;
    }
}

FileNode *findChild(FileNode *parent, const char *name) {
    if (!parent->firstChild) return NULL;
    FileNode *it = parent->firstChild;
    do {
        if (strcmp(it->name, name) == 0) return it;
        it = it->next;
    } while (it != parent->firstChild);
    return NULL;
}

void removeChild(FileNode *parent, FileNode *child) {
    if (!parent->firstChild) return;
    if (child->next == child) parent->firstChild = NULL;
    else {
        if (parent->firstChild == child) parent->firstChild = child->next;
        child->prev->next = child->next;
        child->next->prev = child->prev;
    }
}

void makeDirectory(FileNode *currentDir, const char *name) {
    if (findChild(currentDir, name)) {
        printf("Directory already exists\n");
        return;
    }
    FileNode *dir = (FileNode *)malloc(sizeof(FileNode));
    strcpy(dir->name, name);
    dir->isDirectory = 1;
    dir->blockPointers = NULL;
    dir->blocksAllocated = 0;
    dir->contentSize = 0;
    dir->firstChild = NULL;
    insertChild(currentDir, dir);
    printf("Directory '%s' created.\n", name);
}

void removeDirectory(FileNode *currentDir, const char *name) {
    FileNode *dir = findChild(currentDir, name);
    if (!dir || !dir->isDirectory) {
        printf("Directory not found\n");
        return;
    }
    if (dir->firstChild) {
        printf("Directory not empty\n");
        return;
    }
    removeChild(currentDir, dir);
    free(dir);
    printf("Directory '%s' removed.\n", name);
}

void listDirectory(FileNode *currentDir) {
    if (!currentDir->firstChild) {
        printf("(empty)\n");
        return;
    }
    FileNode *it = currentDir->firstChild;
    do {
        printf(it->isDirectory ? "%s/  " : "%s  ", it->name);
        it = it->next;
    } while (it != currentDir->firstChild);
    printf("\n");
}

void changeDirectory(FileNode **currentDir, const char *name) {
    if (strcmp(name, "..") == 0) {
        if ((*currentDir)->parent) *currentDir = (*currentDir)->parent;
        return;
    }
    FileNode *dir = findChild(*currentDir, name);
    if (!dir || !dir->isDirectory) {
        printf("Directory not found\n");
        return;
    }
    *currentDir = dir;
}

void printWorkingDirectory(FileNode *dir) {
    if (!dir) return;
    if (dir->parent) {
        printWorkingDirectory(dir->parent);
        printf("/%s", dir->name);
    } else {
        printf("/");
    }
}
