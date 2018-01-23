/*
 * Purpose: Contains all the patches
 * with each call to memory related functions
 * being documented to ease debugging
 *
 */

#include "patches.h"

#define Nop(add, size, reason) if(!NopMemory(add, size, reason)) return FALSE;
#define Set(add, size, buffer, reason) if(!SetMemory(add, size, buffer, reason)) return FALSE;
#define Hook(add, func, reason) if(!HookFunc(add, func, reason)) return FALSE;

static const unsigned char twoByteJmp = 0xEB;
BOOL FreadHook();
BOOL OpenFileFromDisk();
DWORD MyGetFileSize(FILE *fp);

BOOL DisableIntros(){

	Nop(0x004707BE, 0x004707C3 - 0x004707BE, "Disable wrapper for Bink functions.")
	return TRUE;
}

/************************************************
 *						*
 * 		FILE LOADER 			*
 * 				 		*
 ************************************************/

typedef struct {
	char name[0x20];
	DWORD crc;
	DWORD compressed;
	DWORD fileOffset;
	DWORD uncompressedSize;
	DWORD compressedSize;
}PkrFile;

BOOL FileLoader(){

	Nop(0x00519376, 5,"Disable memory allocation")
	Set(0x0051938B, 1, &twoByteJmp, "Disable check after memory allocation")
	Hook(0x005193C5, (DWORD)FreadHook, "FreadWrapper patch")

	Nop(0x005193DC, 5, "Disable free after fread fail")
	//Nop(0x00519407, 5, "Disable the decompression routine")
	Nop(0x0051941E, 5, "Disable CRC Check")
	Set(0x0051942D, 1, &twoByteJmp, "Disable jnz after CRC Check")

	return TRUE;
}

char *fileName;
char *fileDirectory;
unsigned char **buffer; //buffer where the file will be stored
PkrFile *curPkr = NULL;

//Typedefs
typedef void* (*newP)(DWORD size);
newP new = (void*)0x00529BA2;
typedef void* (*deleteP)(void *address);
deleteP delete = (void*)0x00529BB0;

//Recreate the original fread wrapper
typedef void* (*lockFileP)(FILE *fp);
lockFileP lockFile = (void*)0x0052BEBD;
typedef void* (*unlockFileP)(FILE *fp);
unlockFileP unlockFile = (void*)0x0052BF0F;
typedef void* (*freadP)(void *buffer, DWORD size, DWORD cnt, FILE *fp);
freadP freadSpidey = (void*)0x0052AA25;

BOOL OpenFileFromDisk(PVOID unused, DWORD compressedSize, DWORD unused1, FILE *pkr){

	static char path[64];
	strcpy(path, fileDirectory);
	strcat(path , fileName);
	FILE *fp = fopen(path, "rb");
	if(!fp){

		printf("Will open from the pkr %s %d\n", fileName, compressedSize);
		*buffer = new(compressedSize);
		if(!(*buffer)){
			printf("Wasn't able to create a buffer for the file being read off the PKR\n");
			return FALSE;
		}

		lockFile(pkr);
		if(!freadSpidey(*buffer, compressedSize, 1, pkr)){
			printf("Couldn't read file off pkr: %s\n", fileName);
			delete(*buffer);
			return FALSE;
		}
		unlockFile(pkr);

		return TRUE;
	}

	DWORD fileSize = MyGetFileSize(fp);
	*buffer = new(fileSize);
	if(!(*buffer)){
		printf("Couldn't allocate space file: %s\n", fileName);
		fclose(fp);
		return FALSE;
	}

	if(!fread(*buffer, fileSize, 1, fp)){
		printf("Error reading file: %s\n", fileName);
		fclose(fp);
		delete(*buffer);
		return FALSE;
	}
	fclose(fp);

	printf("File has been loaded: %s %p\n", curPkr->name, *buffer);
	curPkr->uncompressedSize = fileSize;
	curPkr->compressed = 0xFFFFFFFE;

	return TRUE;
}

DWORD MyGetFileSize(FILE *fp){

	fseek(fp, 0, SEEK_END);
	DWORD size = ftell(fp);
	rewind(fp);
	return size;
}
