/*
 * Purpose: Contains all the patches
 * with each call to memory related functions
 * being documented to ease debugging
 *
 */

#include "patches.h"

#define Nop(add, size, reason) if(!NopMemory(add, size, reason)) return FALSE;
#define Set(add, size, buffer, reason) if(!SetMemory(add, size, buffer, reason)) return FALSE;

static const unsigned char twoByteJmp = 0xEB;
BOOL CrcHook();

BOOL DisableIntros(){

	Nop(0x004707BE, 0x004707C3 - 0x004707BE, "Disable wrapper for Bink functions.")
	return TRUE;
}

BOOL FileLoader(){

	Nop(0x00519376, 5, "Disable buffer allocation")
	Set(0x0051938B, 1, &twoByteJmp, "Patch jnz after buffer allocation")
	//Nop(0x005193AD, 5,"Disable fseek")
	//Nop(0x005193C5, 5, "Disable FreadWrapper")
	//Hooking here might be actually better since I have access to the handle of the file
	//which I can use to load the file if it fails loading from the disk
	Set(0x005193D0, 1, &twoByteJmp, "Patch jnz after Fread wrapper")
	Nop(0x00519407, 5, "Disable the decompression routine")

	DWORD crcHookOff = (DWORD)&CrcHook - (DWORD)0x0051941E - 5;
	Set(0x0051941F, 4, (PVOID)&crcHookOff, "Hooks CrcCheck")
	
	return TRUE;
}

PVOID LoadFromHandle(FILE *fp, DWORD size){

	PVOID buffer = malloc(size);
	if(!buffer){
		MessageBoxA(NULL, "Was unable to create buffer", "Closing", 0);
		return NULL;
	}

	if(!fread(buffer, size, 1, fp)){
		MessageBoxA(NULL, "Was unable to read file from handle", "Closing", 0);
		free(buffer);
		return NULL;
	}

	return buffer;


}
