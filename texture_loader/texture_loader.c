#include "memory.h"

BOOL ApplyHooks();

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID reserverd){
	if(fdwReason == DLL_PROCESS_ATTACH){

		//Console is now allocated only with -console directive
		//Not really
		char *args = GetCommandLine();
		if (!GetModuleHandle("binkw32.dll")){
			printf("Original binkw32 doesn't seem to be loaded...Quitting");
			MessageBoxA(NULL, "Error", "Coudln't get an handle to the original binkw32", 0);
			return FALSE;
		}

		return ApplyHooks();
	}

	return TRUE;
}


BOOL ApplyHooks(){

	//Disable initial intros
	if (!NopMemory(0x004707BE, 0x004707C3 - 0x004707BE, "Disables wrapper for Bink functions."))
		return FALSE;
	if (!NopMemory(0x004707C8, 0x004707CE - 0x004707C8, "Unknown, related to some global var"))
		return FALSE;
	
	return TRUE;

}
