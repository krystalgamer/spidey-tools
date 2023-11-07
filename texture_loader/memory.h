#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <windows.h>

BOOL ChangeAddressPerms(DWORD start, DWORD size, DWORD newPerms, DWORD *oldProtect, const char *reason);
BOOL MakeAddressRW(DWORD start, DWORD size, DWORD *oldProtect, const char *reason);
BOOL NopMemory(DWORD addr, DWORD size, const char *reason);
BOOL SetMemory(DWORD addr, DWORD size, const unsigned char *buffer, const char *reason);
BOOL HookFunc(DWORD callAdd, DWORD funcADd, const unsigned char *reason);

#endif
