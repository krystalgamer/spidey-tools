#include "patches.h"
#include "forwards.h"

BOOL ApplyHooks();

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID reserverd){

	if(fdwReason == DLL_PROCESS_ATTACH){

		
		//Console is now allocated only with -console directive
		AllocConsole();
		if(!freopen("CONOUT$", "w", stdout)){
			MessageBoxA(NULL, "Error", "Couldn't allocate console...Closing", 0);
			return FALSE;
		}

	//	//Not really
	//	char *args = GetCommandLine();
		if (!GetModuleHandle("binkw32.dll")){
			printf("Original binkw32 doesn't seem to be loaded...Quitting");
			MessageBoxA(NULL, "Error", "Couldn't get an handle to the original binkw32", 0);
			return FALSE;
		}

		//return ApplyHooks();
	}

	return TRUE;
}


BOOL ApplyHooks(){

	return (DisableIntros());
}
