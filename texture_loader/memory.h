#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

BOOL ChangeAddressPerms(DWORD start, DWORD size, DWORD newPerms, DWORD *oldProtect, const char *reason);
BOOL MakeAddressRW(DWORD start, DWORD size, DWORD *oldProtect, const char *reason);
void NopMemory(DWORD addr, DWORD size, const char *reason);
void SetMemory(DWORD addr, DWORD size, const unsigned char *buffer, const char *reason);
void HookFunc(DWORD callAdd, DWORD funcADd, const unsigned char *reason);

#endif
