#include "md5.h"
#include "patches.h"
#include "forwards.h"

#define CTL_VERSION "0.4"

BOOL ApplyHooks();
BOOL CheckBinkwVersion();

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID reserverd){

	if(fdwReason == DLL_PROCESS_ATTACH){

		//Console is now allocated only with -console directive
		char *args = GetCommandLine();

		if(strstr(args, " -console")){
			AllocConsole();
			if(!freopen("CONOUT$", "w", stdout)){
				MessageBoxA(NULL, "Error", "Couldn't allocate console...Closing", 0);
				return FALSE;
			}
		}

		printf("Custom texture loader v%s\n", CTL_VERSION);

		if(!CheckBinkwVersion()){
			puts("Wrong binkw32 version.. Please download the correct version");
			return FALSE;
		}

		if (!GetModuleHandle("binkw32.dll")){
			printf("Original binkw32 doesn't seem to be loaded...Quitting");
			MessageBoxA(NULL, "Error", "Couldn't get an handle to the original binkw32", 0);
			return FALSE;
		}

		return ApplyHooks();
	}
	else if(fdwReason == DLL_PROCESS_DETACH)
		FreeConsole();

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


BOOL ApplyHooks(){

	return (DisableIntros() && FileLoader() && TextureLoader());
}
