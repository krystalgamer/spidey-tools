/*
 * Purpose: contains some memory editing routines 
 * such as NopMemory and WriteMemory
 */

#include "memory.h"
#include "log.h"

BOOL ChangeAddressPerms(DWORD start, DWORD size, DWORD newPerms, DWORD *oldProtect, const char *reason) {

	DWORD backupOldProtect;

	BOOL ret = VirtualProtect(
		(LPVOID)start,
		size,
		newPerms,
		oldProtect == NULL ? &backupOldProtect : oldProtect);

	if (TRUE == ret && reason) {
		DebugPrintf("Successfully changed permissions for %08X. Reason: %s\n", start, reason);
	}

	return ret;
}

BOOL MakeAddressRW(DWORD start, DWORD size, DWORD *oldProtect, const char *reason) {
	return ChangeAddressPerms(
		start,
		size,
		PAGE_READWRITE,
		oldProtect,
		reason);
}


void NopMemory(DWORD address, DWORD size, const char *reason){

	//Nop memory
	memset((void*)address, 0x90, size);

	if(reason)
		DebugPrintf("Successfully patched address:%08X Reason: %s\n", address ,reason);
}

void SetMemory(DWORD address, DWORD size, const unsigned char *buffer, const char *reason){

	//Nop memory
	memcpy((void*)address, buffer, size);


	if(reason)
		DebugPrintf("Successfully patched address:%08X Reason: %s\n", address ,reason);
}

/*
 * callAdd - the address that contains the CALL XXXX
 * funcAdd - Address of the function
 *
 */

void HookFunc(DWORD callAdd, DWORD funcAdd, const unsigned char *reason){
	
	//Only works for E9 hooks	
	DWORD jmpOff = funcAdd - callAdd - 5;
	
	SetMemory(callAdd + 1, 4, (unsigned char *)&jmpOff, NULL);

	if(reason)
		DebugPrintf("Hook: %08X -  %s\n", callAdd, reason);
}
