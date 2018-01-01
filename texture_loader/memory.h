#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <windows.h>

BOOL NopMemory(DWORD addr, DWORD size, const char *reason);
BOOL SetMemory(DWORD addr, DWORD size, const unsigned char *buffer, const char *reason);

#endif
