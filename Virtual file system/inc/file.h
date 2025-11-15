#ifndef FILE_H
#define FILE_H

#include "directory.h"

void createFile(FileNode *currentDir, const char *name);
void writeFile(FileNode *currentDir, const char *name, const char *data);
void readFile(FileNode *currentDir, const char *name);
void deleteFile(FileNode *currentDir, const char *name);

#endif // FILE_H
