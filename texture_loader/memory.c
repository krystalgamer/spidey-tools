/*
 * Purpose: contains some memory editing routines 
 * such as NopMemory and WriteMemory
 */

#include "memory.h"

BOOL NopMemory(DWORD address, DWORD size, CHAR* reason){

	DWORD oldProtect;
	PVOID cAddress = (PVOID)address; //Casted address should be inlined

	//Make it writable so we can edit it
	if(!VirtualProtect(cAddress, size, PAGE_EXECUTE_READWRITE, &oldProtect)){
		printf("There was a problem removing the protection from the address %08X", address);
		MessageBoxA(NULL, "Error", "Coudln't remove the protection from a memory location. Please check the console for more info.", 0);
		return FALSE;
	}

	//Nop memory
	memset(cAddress, 0x90, size);

	//Restore the protection
	if(!VirtualProtect(cAddress, size, oldProtect, &oldProtect)){
		printf("There was a problem restoring the protection from the address %08X", address);
		MessageBoxA(NULL, "Error", "Coudln't restore the protection from a memory location. Please check the console for more info.", 0);
		return FALSE;
	}

	printf("Successfully patched address:%08X Reason: %s", reason);

	return TRUE;
}

