#define _CRT_SECURE_NO_WARNINGS

#include "new_ep.h"
#include "memory.h"
#include <stdbool.h>
#include "cJSON.h"
#include "patches.h"
#include "console.h"
#include <stdlib.h>

#include "log.h"

#define CTL_VERSION "0.9.2"

typedef int (*OriginalEntryPoint_t)(void);
OriginalEntryPoint_t OriginalEntryPoint = (OriginalEntryPoint_t)0x0052B46F;

typedef struct{
	bool no_intros;
	bool no_fmvs;
	bool texture_loader;
	bool file_loader;
	bool console;
	bool psx_graphics;
	bool frame_counter;
	bool frame_limiter;
	bool fix_bugs;
	bool unlock_everything;
	bool skip_useless_file_load;
	bool add_restart_option;
} Settings;

void EnsureBinkw32IsLoaded(void) {

	if (!GetModuleHandle("binkw32.dll")){
		puts("Original binkw32 doesn't seem to be loaded...");
	}
}

typedef int (*InstallSM2000Patches_t)(void);

/*
* Loads plugins external from this project
*/
static BOOL ApplyThirdPartyPatches(void) {

	WIN32_FIND_DATA findData;
	memset(&findData, 0, sizeof(findData));

	HANDLE findHandle;
	findHandle = FindFirstFileA("plugins\\*.dll", &findData);

	if (findHandle == INVALID_HANDLE_VALUE) {
		goto cleanup;
	}

	char thirdPartyPath[MAX_PATH];
	memset(thirdPartyPath, 0, sizeof(thirdPartyPath));

	do {
		DebugPrintf("Loading %s...", findData.cFileName);

		snprintf(thirdPartyPath, sizeof(thirdPartyPath), "plugins\\%s", findData.cFileName);
		HMODULE loadedLibary = LoadLibraryA(thirdPartyPath);

		if (loadedLibary == NULL) {
			puts("Loading failed");
			continue;
		}

		InstallSM2000Patches_t currentInstaller = (InstallSM2000Patches_t)GetProcAddress(loadedLibary, "InstallSM2000Patches");

		if (currentInstaller == NULL) {
			currentInstaller = (InstallSM2000Patches_t)GetProcAddress(loadedLibary, "?InstallSM2000Patches@@YAHXZ");
		}

		if (currentInstaller == NULL) {
			puts("Module missing InstallSM2000Patches unloading it");
			FreeLibrary(loadedLibary);
			continue;
		}

		puts("Success, will now run it!");
		if (currentInstaller()) {
			puts("It failed, unloading it");
			FreeLibrary(loadedLibary);
		}
	}
	while(FALSE != FindNextFileA(findHandle, &findData));

	cleanup:
	FindClose(findHandle);
	return TRUE;
}

#define DO_OR_QUIT(x) { if(x == FALSE) return FALSE; }
static BOOL ApplyMyPatches(const Settings *settings){

	if (settings == NULL) {
		return FALSE;
	}

	if (settings->console) {

		if (AllocateConsole()) {
			MessageBoxA(NULL, "Error", "Couldn't allocate console...Closing", 0);
			return FALSE;
		}
	}

	if (settings->psx_graphics) {
		DO_OR_QUIT(LowRes());
	}

	if (settings->no_intros) {
		DO_OR_QUIT(DisableIntros());
	}

	if (settings->no_fmvs) {
		DO_OR_QUIT(DisableFmvs());
	}

	if (settings->file_loader) {
		DO_OR_QUIT(FileLoader());
	}

	if (settings->texture_loader) {
		DO_OR_QUIT(TextureLoader());
	}

	if (settings->frame_counter) {
		DO_OR_QUIT(FrameCounter());
	}

	if (settings->frame_limiter) {
		DO_OR_QUIT(FrameLimiter());
	}

	if (settings->fix_bugs) {
		DO_OR_QUIT(FixBugs());
	}

	if (settings->unlock_everything) {
		DO_OR_QUIT(UnlockEverything());
	}

	if (settings->skip_useless_file_load) {
		DO_OR_QUIT(PatchSFXInitAtStart());
	}

	if (settings->add_restart_option) {
		DO_OR_QUIT(AddRestartOption());
	}
	

#ifdef _DEBUG
	TestingGround();
#endif

	return TRUE;
}

static BOOL ApplyPatches(const Settings* settings) {

	DWORD textStart = 0x00401000;
	DWORD textSize = 0x0053B000 - textStart;
	DWORD textOldPerms;

	DWORD iDataStart = 0x0053B000;
    DWORD iDataSize = 0x0053B230 - iDataStart;
	DWORD iDataOldPerms;

	DWORD rDataStart = 0x0053B230;
	DWORD rDataSize = 0x00546000 - rDataStart;
	DWORD rDataOldPerms;


	DO_OR_QUIT(MakeAddressRW(textStart, textSize, &textOldPerms, "Changing .text to be rw"));
	DO_OR_QUIT(MakeAddressRW(iDataStart, iDataSize, &iDataOldPerms, "Changing .idata to be rw"));
	DO_OR_QUIT(MakeAddressRW(rDataStart, rDataSize, &rDataOldPerms, "Changing .rdata to be rw"));

	DO_OR_QUIT(ApplyMyPatches(settings));
	DO_OR_QUIT(ApplyThirdPartyPatches());

	DO_OR_QUIT(ChangeAddressPerms(textStart, textSize, textOldPerms, NULL, "Restoring .text perms"));
	DO_OR_QUIT(ChangeAddressPerms(iDataStart, iDataSize, iDataOldPerms, NULL, "Restoring .idata perms"));
	DO_OR_QUIT(ChangeAddressPerms(rDataStart, rDataSize, rDataOldPerms, NULL, "Restoring .rdata perms"));

	return TRUE;
}

static int GetJsonBool(cJSON* json, const char* bool_name, bool* output) {
	cJSON *object = cJSON_GetObjectItemCaseSensitive(json, bool_name);

	if (object == NULL) {
		return 0;
	}

	if (!cJSON_IsBool(object)) {
		return 0;
	}

	if (cJSON_IsTrue(object)) {
		*output = true;
		return 1;
	}

	if (cJSON_IsFalse(object)) {
		*output = false;
		return 1;
	}

	return 0;
}

static void AddSettingToJsonObject(cJSON *object, const char *name, bool setting){

	if (setting) {
		cJSON_AddTrueToObject(object, name);
	}
	else {
		cJSON_AddFalseToObject(object, name);
	}
}

static void WriteSettingsToDisk(const Settings *settings) {

	FILE* fp = fopen("sm2000.json", "w");

	if (fp == NULL) {
		return;
	}

	cJSON* json = cJSON_CreateObject();
	AddSettingToJsonObject(json, "no_intros", settings->no_intros);
	AddSettingToJsonObject(json, "no_fmvs", settings->no_fmvs);
	AddSettingToJsonObject(json, "psx_graphics", settings->psx_graphics);
	AddSettingToJsonObject(json, "console", settings->console);
	AddSettingToJsonObject(json, "texture_loader", settings->texture_loader);
	AddSettingToJsonObject(json, "file_loader", settings->file_loader);
	AddSettingToJsonObject(json, "frame_counter", settings->frame_counter);
	AddSettingToJsonObject(json, "frame_limiter", settings->frame_limiter);
	AddSettingToJsonObject(json, "fix_bugs", settings->fix_bugs);
	AddSettingToJsonObject(json, "unlock_everything", settings->unlock_everything);
	AddSettingToJsonObject(json, "skip_useless_file_load", settings->skip_useless_file_load);
	AddSettingToJsonObject(json, "add_restart_option", settings->add_restart_option);

	char *content = cJSON_Print(json);
	fputs(content, fp);
	fclose(fp);
	cJSON_Delete(json);
}

static void ReadSettings(Settings* settings) {

	if (settings == NULL) {
		return;
	}

	FILE *fp = fopen("sm2000.json", "rb");

	if (fp == NULL) {
		WriteSettingsToDisk(settings);
		return;
	}

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (fileSize <= 0)
	{
		WriteSettingsToDisk(settings);
		fclose(fp);
		return;
	}

	char *input = malloc(fileSize);
	if (input == NULL)
	{
		MessageBoxA(NULL, "Alloc fail", "Coudln't allocate buffer to read sm2000.json", 0);
		return;
	}


	fread(input, fileSize, 1, fp);
	fclose(fp);

	cJSON* json = cJSON_ParseWithLength(input, fileSize);
	free(input);

	if (json == NULL) {
		const char* error = cJSON_GetErrorPtr();
		error = error == NULL ? "no error" : error;
		MessageBoxA(NULL, error, "Invalid JSON", 0);
		exit(420);
	}

	GetJsonBool(json, "no_intros", &settings->no_intros);
	GetJsonBool(json, "no_fmvs", &settings->no_fmvs);
	GetJsonBool(json, "psx_graphics", &settings->psx_graphics);
	GetJsonBool(json, "console", &settings->console);
	GetJsonBool(json, "texture_loader", &settings->texture_loader);
	GetJsonBool(json, "file_loader", &settings->file_loader);
	GetJsonBool(json, "frame_counter", &settings->frame_counter);
	GetJsonBool(json, "frame_limiter", &settings->frame_limiter);
	GetJsonBool(json, "fix_bugs", &settings->fix_bugs);
	GetJsonBool(json, "unlock_everything", &settings->unlock_everything);
	GetJsonBool(json, "skip_useless_file_load", &settings->skip_useless_file_load);
	GetJsonBool(json, "add_restart_option", &settings->add_restart_option);

	cJSON_Delete(json);
}

/*
* New entry point where plugins will be loaded
*/
static int NewEntryPoint() {


	Settings settings = {
		.console = true,
		.file_loader = true,
		.texture_loader = true,
		.no_intros = true,
		.no_fmvs = false,
		.psx_graphics = false,
		.frame_counter = false,
		.frame_limiter = false,
		.fix_bugs = true,
		.unlock_everything = false,
		.skip_useless_file_load = true,
		.add_restart_option = true,
	};

	ReadSettings(&settings);

	if (ApplyPatches(&settings) == FALSE) {
		return FALSE;
	}

	EnsureBinkw32IsLoaded();

	DebugPrintf("Custom texture loader v%s\n", CTL_VERSION);

	unsigned char originalBytes[6] = {
		0x55, 0x8B, 0xEC, 0x6A, 0xFF, 0x68
	};

	DWORD oldPerms;
	MakeAddressRW((DWORD)OriginalEntryPoint, 1, &oldPerms, NULL);
	SetMemory((DWORD)OriginalEntryPoint, sizeof(originalBytes), originalBytes, "Restoring entry point");
	ChangeAddressPerms((DWORD)OriginalEntryPoint, 1, oldPerms, NULL, NULL);

	return OriginalEntryPoint();
}

/*
* Changes spider-man 2000 entry to my custom one
*/
void ChangeEntryPoint(void) {

	unsigned char jmpAndRet[6] = {
		0x68, 0x00, 0x00, 0x00 ,0x00,
		0xC3
	};

	*(DWORD*)(&jmpAndRet[1]) = (DWORD)&NewEntryPoint;

	DWORD oldPerms;
	MakeAddressRW((DWORD)OriginalEntryPoint, sizeof(jmpAndRet), &oldPerms, "Changing permissions to patch EP");
	SetMemory((DWORD)OriginalEntryPoint, sizeof(jmpAndRet), jmpAndRet, "Change entry to must new one");
	ChangeAddressPerms((DWORD)OriginalEntryPoint, sizeof(jmpAndRet), oldPerms, NULL, "Restoring permissions in EP");
}
