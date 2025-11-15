#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdio.h>

#define BLOCK_SIZE 512
#define MAX_NAME_LENGTH 50
#define MAX_BLOCKS 1024

typedef struct FileNode
{
    char name[MAX_NAME_LENGTH];
    int isDirectory;
    int *blockPointers;
    int blocksAllocated;
    int contentSize;
    struct FileNode *parent;
    struct FileNode *next;
    struct FileNode *prev;
    struct FileNode *firstChild;
} FileNode;

typedef struct FreeBlock
{
    int blockIndex;
    struct FreeBlock *prev;
    struct FreeBlock *next;
} FreeBlock;

void insertFreeBlockAtTail(const int index);
int allocateBlock();
void freeBlock(const int index);

void insertChild(FileNode *parent, FileNode *child);
FileNode *findChild(FileNode *parent, const char *name);
void removeChild(FileNode *parent, FileNode *child);

void makeDirectory(FileNode *currentDir, const char *name);
void removeDirectory(FileNode *currentDir, const char *name);
void listDirectory(FileNode *currentDir);
void changeDirectory(FileNode **currentDir, const char *name);
void printWorkingDirectory(FileNode *currentDir);

#endif // DIRECTORY_H
