/*
 * Purpose: contains some memory editing routines 
 * such as NopMemory and WriteMemory
 */

#include "memory.h"

BOOL NopMemory(DWORD address, DWORD size, const char *reason){

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

	if(reason)
		printf("Successfully patched address:%08X Reason: %s\n", cAddress ,reason);

	return TRUE;
}

BOOL SetMemory(DWORD address, DWORD size, const unsigned char *buffer, const char *reason){

	DWORD oldProtect;
	PVOID cAddress = (PVOID)address; //Casted address should be inlined

	//Make it writable so we can edit it
	if(!VirtualProtect(cAddress, size, PAGE_EXECUTE_READWRITE, &oldProtect)){
		printf("There was a problem removing the protection from the address %08X", address);
		MessageBoxA(NULL, "Error", "Coudln't remove the protection from a memory location. Please check the console for more info.", 0);
		return FALSE;
	}

	//Nop memory
	memcpy(cAddress, buffer, size);

	//Restore the protection
	if(!VirtualProtect(cAddress, size, oldProtect, &oldProtect)){
		printf("There was a problem restoring the protection from the address %08X", address);
		MessageBoxA(NULL, "Error", "Coudln't restore the protection from a memory location. Please check the console for more info.", 0);
		return FALSE;
	}

	if(reason)
		printf("Successfully patched address:%08X Reason: %s\n", cAddress ,reason);

	return TRUE;
}

/*
 * callAdd - the address that contains the CALL XXXX
 * funcAdd - Address of the function
 *
 */

BOOL HookFunc(DWORD callAdd, DWORD funcAdd, const unsigned char *reason){
	
	//Only works for E9 hooks	
	DWORD jmpOff = funcAdd - callAdd - 5;
	
	if(!SetMemory(callAdd + 1, 4, (unsigned char *)&jmpOff, NULL)){
		printf("Was unable to hook address: %08X\n", callAdd);
		return FALSE;
	}

	if(reason)
		printf("Hook: %08X -  %s\n", callAdd, reason);
	
	return TRUE;
}
