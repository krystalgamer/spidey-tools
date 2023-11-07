#include "md5.h"
#include "forwards.h"
#include "new_ep.h"
#include "console.h"
#include <stdio.h>
#include <windows.h>

BOOL CheckBinkwVersion();

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID reserverd){

	if(fdwReason == DLL_PROCESS_ATTACH){
		AllocateConsole();
		ChangeEntryPoint();
		return TRUE;
	}
	else if (fdwReason == DLL_PROCESS_DETACH) {
		ReleaseConsole();
	}

	return TRUE;
}

unsigned char binkwChecksum[16] = {
	0x62, 0xBe, 0xE9, 0x3E,
	0xF9, 0xE3, 0x97, 0x4A,
	0x9E, 0x2C, 0x59, 0xF2,
	0xA8, 0x32, 0xB0, 0xA5
};
unsigned char binkwBuffer[1024];
BOOL CheckBinkwVersion(){
	
	FILE *fp;
	fp = fopen("binkw32_.dll", "rb");
	if(!fp){
		MessageBoxA(NULL, "Error", "Couldn't open binkw32_.dll.. Quitting", 0);
		puts("Couldn't open binkw32_.dll.. Quitting");
		return FALSE;
	}

	MD5_CTX ctx;
	MD5_Init(&ctx);

	DWORD bytesRead = 0;
	while((bytesRead = fread(binkwBuffer, 1, 1024, fp)) != 0)
		MD5_Update(&ctx, binkwBuffer, bytesRead);

	if(!feof(fp)){
		MessageBoxA(NULL, "Error", "There was an error reading the file", 0);
		puts("There was an error reading the file");
		return FALSE;
	}
	fclose(fp);

	unsigned char checksum[16];
	MD5_Final((void*)&checksum, &ctx);
	if(memcmp(checksum, binkwChecksum, 16)){
		MessageBoxA(NULL, "Error", "Wrong binkw version or corrupted!", 0);
		puts("Wrong binkw version or corrupted!");
		return FALSE;
	}

	return TRUE; 
}
