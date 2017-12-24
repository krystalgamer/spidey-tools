#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <windows.h>

BOOL NopMemory(DWORD addr, DWORD size, CHAR* reason);
BOOL SetMemory(DWORD addr, UCHAR *buffer, DWORD size);

#endif
