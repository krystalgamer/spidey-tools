#include <windows.h>
#include <stdio.h>

#pragma warning(disable:4996)

BOOL NopMemory(DWORD address, DWORD size);
BOOL SetMemory(DWORD address, UCHAR *buffer, DWORD bufSize);

#pragma comment (linker, "/export:_BinkBufferBlit@12=binkw32_._BinkBufferBlit@12,@1")
#pragma comment (linker, "/export:_BinkBufferCheckWinPos@12=binkw32_._BinkBufferCheckWinPos@12,@2")
#pragma comment (linker, "/export:_BinkBufferClear@8=binkw32_._BinkBufferClear@8,@3")
#pragma comment (linker, "/export:_BinkBufferClose@4=binkw32_._BinkBufferClose@4,@4")
#pragma comment (linker, "/export:_BinkBufferGetDescription@4=binkw32_._BinkBufferGetDescription@4,@5")
#pragma comment (linker, "/export:_BinkBufferGetError@0=binkw32_._BinkBufferGetError@0,@6")
#pragma comment (linker, "/export:_BinkBufferLock@4=binkw32_._BinkBufferLock@4,@7")
#pragma comment (linker, "/export:_BinkBufferOpen@16=binkw32_._BinkBufferOpen@16,@8")
#pragma comment (linker, "/export:_BinkBufferSetDirectDraw@8=binkw32_._BinkBufferSetDirectDraw@8,@9")
#pragma comment (linker, "/export:_BinkBufferSetHWND@8=binkw32_._BinkBufferSetHWND@8,@10")
#pragma comment (linker, "/export:_BinkBufferSetOffset@12=binkw32_._BinkBufferSetOffset@12,@11")
#pragma comment (linker, "/export:_BinkBufferSetResolution@12=binkw32_._BinkBufferSetResolution@12,@12")
#pragma comment (linker, "/export:_BinkBufferSetScale@12=binkw32_._BinkBufferSetScale@12,@13")
#pragma comment (linker, "/export:_BinkBufferUnlock@4=binkw32_._BinkBufferUnlock@4,@14")
#pragma comment (linker, "/export:_BinkCheckCursor@20=binkw32_._BinkCheckCursor@20,@15")
#pragma comment (linker, "/export:_BinkClose@4=binkw32_._BinkClose@4,@16")
#pragma comment (linker, "/export:_BinkCloseTrack@4=binkw32_._BinkCloseTrack@4,@17")
#pragma comment (linker, "/export:_BinkCopyToBuffer@28=binkw32_._BinkCopyToBuffer@28,@18")
#pragma comment (linker, "/export:_BinkDDSurfaceType@4=binkw32_._BinkDDSurfaceType@4,@19")
#pragma comment (linker, "/export:_BinkDX8SurfaceType@4=binkw32_._BinkDX8SurfaceType@4,@20")
#pragma comment (linker, "/export:_BinkDoFrame@4=binkw32_._BinkDoFrame@4,@21")
#pragma comment (linker, "/export:_BinkGetError@0=binkw32_._BinkGetError@0,@22")
#pragma comment (linker, "/export:_BinkGetKeyFrame@12=binkw32_._BinkGetKeyFrame@12,@23")
#pragma comment (linker, "/export:_BinkGetRealtime@12=binkw32_._BinkGetRealtime@12,@24")
#pragma comment (linker, "/export:_BinkGetRects@8=binkw32_._BinkGetRects@8,@25")
#pragma comment (linker, "/export:_BinkGetSummary@8=binkw32_._BinkGetSummary@8,@26")
#pragma comment (linker, "/export:_BinkGetTrackData@8=binkw32_._BinkGetTrackData@8,@27")
#pragma comment (linker, "/export:_BinkGetTrackID@8=binkw32_._BinkGetTrackID@8,@28")
#pragma comment (linker, "/export:_BinkGetTrackMaxSize@8=binkw32_._BinkGetTrackMaxSize@8,@29")
#pragma comment (linker, "/export:_BinkGetTrackType@8=binkw32_._BinkGetTrackType@8,@30")
#pragma comment (linker, "/export:_BinkGoto@12=binkw32_._BinkGoto@12,@31")
#pragma comment (linker, "/export:_BinkIsSoftwareCursor@8=binkw32_._BinkIsSoftwareCursor@8,@32")
#pragma comment (linker, "/export:_BinkLogoAddress@0=binkw32_._BinkLogoAddress@0,@33")
#pragma comment (linker, "/export:_BinkNextFrame@4=binkw32_._BinkNextFrame@4,@34")
#pragma comment (linker, "/export:_BinkOpen@8=binkw32_._BinkOpen@8,@35")
#pragma comment (linker, "/export:_BinkOpenDirectSound@4=binkw32_._BinkOpenDirectSound@4,@36")
#pragma comment (linker, "/export:_BinkOpenMiles@4=binkw32_._BinkOpenMiles@4,@37")
#pragma comment (linker, "/export:_BinkOpenTrack@8=binkw32_._BinkOpenTrack@8,@38")
#pragma comment (linker, "/export:_BinkOpenWaveOut@4=binkw32_._BinkOpenWaveOut@4,@39")
#pragma comment (linker, "/export:_BinkPause@8=binkw32_._BinkPause@8,@40")
#pragma comment (linker, "/export:_BinkRestoreCursor@4=binkw32_._BinkRestoreCursor@4,@41")
#pragma comment (linker, "/export:_BinkService@4=binkw32_._BinkService@4,@42")
#pragma comment (linker, "/export:_BinkSetError@4=binkw32_._BinkSetError@4,@43")
#pragma comment (linker, "/export:_BinkSetFrameRate@8=binkw32_._BinkSetFrameRate@8,@44")
#pragma comment (linker, "/export:_BinkSetIO@4=binkw32_._BinkSetIO@4,@45")
#pragma comment (linker, "/export:_BinkSetIOSize@4=binkw32_._BinkSetIOSize@4,@46")
#pragma comment (linker, "/export:_BinkSetPan@8=binkw32_._BinkSetPan@8,@47")
#pragma comment (linker, "/export:_BinkSetSimulate@4=binkw32_._BinkSetSimulate@4,@48")
#pragma comment (linker, "/export:_BinkSetSoundOnOff@8=binkw32_._BinkSetSoundOnOff@8,@49")
#pragma comment (linker, "/export:_BinkSetSoundSystem@8=binkw32_._BinkSetSoundSystem@8,@50")
#pragma comment (linker, "/export:_BinkSetSoundTrack@4=binkw32_._BinkSetSoundTrack@4,@51")
#pragma comment (linker, "/export:_BinkSetVideoOnOff@8=binkw32_._BinkSetVideoOnOff@8,@52")
#pragma comment (linker, "/export:_BinkSetVolume@8=binkw32_._BinkSetVolume@8,@53")
#pragma comment (linker, "/export:_BinkWait@4=binkw32_._BinkWait@4,@54")
#pragma comment (linker, "/export:_RADTimerRead@0=binkw32_._RADTimerRead@0,@55")
#pragma comment (linker, "/export:_YUV_blit_16a1bpp@52=binkw32_._YUV_blit_16a1bpp@52,@56")
#pragma comment (linker, "/export:_YUV_blit_16a1bpp_mask@52=binkw32_._YUV_blit_16a1bpp_mask@52,@57")
#pragma comment (linker, "/export:_YUV_blit_16a4bpp@52=binkw32_._YUV_blit_16a4bpp@52,@58")
#pragma comment (linker, "/export:_YUV_blit_16a4bpp_mask@52=binkw32_._YUV_blit_16a4bpp_mask@52,@59")
#pragma comment (linker, "/export:_YUV_blit_16bpp@48=binkw32_._YUV_blit_16bpp@48,@60")
#pragma comment (linker, "/export:_YUV_blit_16bpp_mask@48=binkw32_._YUV_blit_16bpp_mask@48,@61")
#pragma comment (linker, "/export:_YUV_blit_24bpp@48=binkw32_._YUV_blit_24bpp@48,@62")
#pragma comment (linker, "/export:_YUV_blit_24bpp_mask@48=binkw32_._YUV_blit_24bpp_mask@48,@63")
#pragma comment (linker, "/export:_YUV_blit_24rbpp@48=binkw32_._YUV_blit_24rbpp@48,@64")
#pragma comment (linker, "/export:_YUV_blit_24rbpp_mask@48=binkw32_._YUV_blit_24rbpp_mask@48,@65")
#pragma comment (linker, "/export:_YUV_blit_32abpp@52=binkw32_._YUV_blit_32abpp@52,@66")
#pragma comment (linker, "/export:_YUV_blit_32abpp_mask@52=binkw32_._YUV_blit_32abpp_mask@52,@67")
#pragma comment (linker, "/export:_YUV_blit_32bpp@48=binkw32_._YUV_blit_32bpp@48,@68")
#pragma comment (linker, "/export:_YUV_blit_32bpp_mask@48=binkw32_._YUV_blit_32bpp_mask@48,@69")
#pragma comment (linker, "/export:_YUV_blit_32rabpp@52=binkw32_._YUV_blit_32rabpp@52,@70")
#pragma comment (linker, "/export:_YUV_blit_32rabpp_mask@52=binkw32_._YUV_blit_32rabpp_mask@52,@71")
#pragma comment (linker, "/export:_YUV_blit_32rbpp@48=binkw32_._YUV_blit_32rbpp@48,@72")
#pragma comment (linker, "/export:_YUV_blit_32rbpp_mask@48=binkw32_._YUV_blit_32rbpp_mask@48,@73")
#pragma comment (linker, "/export:_YUV_blit_UYVY@48=binkw32_._YUV_blit_UYVY@48,@74")
#pragma comment (linker, "/export:_YUV_blit_UYVY_mask@48=binkw32_._YUV_blit_UYVY_mask@48,@75")
#pragma comment (linker, "/export:_YUV_blit_YUY2@48=binkw32_._YUV_blit_YUY2@48,@76")
#pragma comment (linker, "/export:_YUV_blit_YUY2_mask@48=binkw32_._YUV_blit_YUY2_mask@48,@77")
#pragma comment (linker, "/export:_YUV_blit_YV12@52=binkw32_._YUV_blit_YV12@52,@78")
#pragma comment (linker, "/export:_YUV_init@4=binkw32_._YUV_init@4,@79")
#pragma comment (linker, "/export:_radfree@4=binkw32_._radfree@4,@80")
#pragma comment (linker, "/export:_radmalloc@4=binkw32_._radmalloc@4,@81")

UCHAR twoByteJmp = 0xEB;

#pragma pack(push, 1)
typedef struct {
	char name[0x20];
	DWORD crc;
	DWORD compressed;
	DWORD fileOffset;
	DWORD uncompressedSize;
	DWORD compressedSize;
}PkrFile;

char *originalDirectory;
char filePath[100];
DWORD filePathAdd = &filePath[0];//yes...

PVOID *LoadStub(PkrFile *pkrFile) {

	HANDLE hFile = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBoxA(NULL, "Error opening the file", "dammit", 0);
		return NULL;
	}

	DWORD fileSize = GetFileSize(hFile, NULL);
	if (fileSize == INVALID_FILE_SIZE) {
		CloseHandle(hFile);
		MessageBoxA(NULL, "Error getting size", "dammit", 0);
		return NULL;
	}
	pkrFile->uncompressedSize = fileSize;

	UCHAR *buffer = malloc(fileSize);
	if (!buffer) {
		CloseHandle(hFile);
		MessageBoxA(NULL, "Error creating buffer", "dammit", 0);
		return NULL;
	}

	DWORD bytesRead = 0;
	if (!ReadFile(hFile, buffer, fileSize, &bytesRead, NULL)) {
		free(buffer);
		CloseHandle(hFile);
		MessageBoxA(NULL, "Error reading the file", "dammit", 0);
		return NULL;
	}

	if (bytesRead != fileSize) {
		free(buffer);
		CloseHandle(hFile);
		MessageBoxA(NULL, "Not all bytes were read", "dammit", 0);
		return NULL;
	}

	CloseHandle(hFile);
	
	printf("Loaded %s\n", filePath);
	return buffer;

}

char *mystrcpy(char * dest, char *src) {
	return strcpy(dest, src);
}
__declspec(naked) PVOID LoadFile(PkrFile *pkrFile, PVOID loadBuf, DWORD one) {

	__asm {
		//get the directory
		mov eax, [esp + 0xA4]
		mov originalDirectory, eax

		//Setups the path
		push originalDirectory
		push filePathAdd
		call mystrcpy
		add esp, 8

		push 0x20
		push [esp+8]
		push eax //Its in eax from strcpy
		call strncat
		add esp, 0xC

		push [esp+4]
		call LoadStub
		add esp,4
		ret
	}

}

UCHAR retOp = 0xC3;

UINT8 **cheatListCode = NULL;
DWORD maxAddress = 0;

BOOL SetupNewCheatArray(DWORD newCheatCodes) {

	if (!newCheatCodes)
		return FALSE;

	DWORD newCheatListSize = (0x005514A0 - 0x005513E0) + newCheatCodes * 4 * 2;
	//Save the original cheat list
	cheatListCode = malloc(newCheatListSize);
	if (!cheatListCode)
		return FALSE;

	memset(cheatListCode, 0, newCheatListSize);
	memcpy(cheatListCode, 0x005513E0, (DWORD)&cheatListCode[48] - (DWORD)&cheatListCode[0]);
	
	maxAddress = (DWORD)&cheatListCode[48 + (newCheatCodes-1) * 2];
	return TRUE;
}

BOOL AddCheatCode(const char *code, const char *display) {

	static DWORD addedCheatCodes = 0;

	if (maxAddress < cheatListCode[48 + addedCheatCodes * 2]) {
		MessageBoxA(NULL, "TOO MANY", "TOO MANY ADDED", 0);
		return FALSE;
	}

	UINT8 *codeBuf = malloc(strlen(code) + 1);
	if (!codeBuf)
		return FALSE;
	UINT8 *displayBuf = malloc(strlen(display) + 1);
	if (!displayBuf) {
		free(codeBuf);
		return FALSE;
	}

	strcpy(codeBuf, code);
	strcpy(displayBuf, display);
	codeBuf[strlen(code)] = '\0';
	displayBuf[strlen(display)] = '\0';

	cheatListCode[48 + addedCheatCodes * 2] = codeBuf;
	cheatListCode[48 + addedCheatCodes * 2 + 1] = displayBuf;
	addedCheatCodes++;
	return TRUE;
}

BOOL PatchPkrLoad(){

	//Disable initial intros
	if (!NopMemory(0x004707BE, 0x004707C3 - 0x004707BE))
		return FALSE;
	if (!NopMemory(0x004707C8, 0x004707CE - 0x004707C8))
		return FALSE;
	
	//Custom cheat codes
	#define NEW_CHEAT_CODES 3
	if (!SetupNewCheatArray(NEW_CHEAT_CODES))
		return FALSE;
	if (!AddCheatCode("KRYSTAL", "HAS A HUGE DICK"))
		return FALSE;
	if (!AddCheatCode("TRUMP", "LOVES LASAGNA"))
		return FALSE;
	if (!AddCheatCode("ENGLAND", "IS A CITY"))
		return FALSE;

	if (!SetMemory(0x0047C44E, &cheatListCode, 4))
		return FALSE;
	DWORD displayTextAddress = (DWORD)cheatListCode + 4;
	if (!SetMemory(0x0049E5E5, (UCHAR*)&displayTextAddress, 4))
		return FALSE;
	DWORD limitLoop = maxAddress + 4;
	if (!SetMemory(0x0047C468, (UCHAR*)(&limitLoop), 4))
		return FALSE;
	if (!NopMemory(0x0047C3C4, 2))
		return FALSE;

	//Disable buffer allocation of pkr extracted file
	if (!NopMemory(0x000519375, 0x005193A1 - 0x000519375))
		return FALSE;

	//Disable fread of the file and the error
	if (!NopMemory(0x005193C5, 0x005193CA - 0x005193C5))
		return FALSE;
	if (!SetMemory(0x005193D0, &twoByteJmp, 1))
		return FALSE;
	if (!NopMemory(0x005193D2, 0x005193FD - 0x005193D2))
		return FALSE;

	DWORD jmpOffset = (DWORD)&LoadFile - 0x0051940C;
	//Hook file loader function
	if (!SetMemory(0x00519408, &jmpOffset, 4))
		return FALSE;
	//Disable CRC checks
	if (!NopMemory(0x0051942D, 0x00519445 - 0x0051942D))
		return FALSE;
	if (!NopMemory(0x0051941E, 0x00519423 - 0x0051941E))
		return FALSE;
	if (!SetMemory(0x0051AD70, &retOp, 1))
		return FALSE;


	return TRUE;
}

BOOL SetMemory(DWORD address, UCHAR *buffer, DWORD bufSize) {
	
	DWORD oldProtect = 0;
	if (!VirtualProtect(address, bufSize, PAGE_EXECUTE_READWRITE, &oldProtect))
		return FALSE;
	
	memcpy((void*)address, (void*)buffer, bufSize);

	if (!VirtualProtect(address, bufSize, oldProtect, &oldProtect))
		return FALSE;

	return TRUE;
}

BOOL NopMemory(DWORD address, DWORD size) {
	
	DWORD oldProtect = 0;
	if (!VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect))
		return FALSE;
	memset(address, 0x90, size);

	if (!VirtualProtect(address, size, oldProtect, &oldProtect))
		return FALSE;

	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved){

	if(reason == DLL_PROCESS_ATTACH){
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		HMODULE bink = LoadLibraryA("binkw32_.dll");
		if (!bink) {
			MessageBoxA(NULL, "ERROR", "Error loading original bink dll", 0);
			return FALSE;
		}

		return PatchPkrLoad();
	}
}