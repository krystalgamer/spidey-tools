#include "md5.h"
#include "patches.h"
#include "forwards.h"
#include <stdbool.h>
#include "cJSON.h"

#define CTL_VERSION "0.5"

BOOL ApplyHooks();
BOOL CheckBinkwVersion();
void ReadSettings();

typedef struct{
	bool no_videos;
	bool texture_loader;
	bool file_loader;
	bool console;
	bool psx_graphics;

} Settings;

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID reserverd){

	if(fdwReason == DLL_PROCESS_ATTACH){

		Settings settings = {
			.console = true,
			.file_loader = true,
			.texture_loader = true,
			.no_videos = false,
			.psx_graphics = false,
		};

		ReadSettings(&settings);

		if (ApplyHooks(&settings) == FALSE) {
			return FALSE;
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

		return TRUE;
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


#define DO_OR_QUIT(x) { if(x == FALSE) return FALSE; }
BOOL ApplyHooks(const Settings *settings){

	if (settings == NULL) {
		return FALSE;
	}

	if (settings->console) {
		AllocConsole();
		if(!freopen("CONOUT$", "w", stdout)){
			MessageBoxA(NULL, "Error", "Couldn't allocate console...Closing", 0);
			return FALSE;
		}
	}

	if (settings->psx_graphics) {
		DO_OR_QUIT(LowRes());
	}

	if (settings->no_videos) {
		DO_OR_QUIT(DisableIntros());
	}

	if (settings->file_loader) {
		DO_OR_QUIT(FileLoader());
	}

	if (settings->texture_loader) {
		DO_OR_QUIT(TextureLoader());
	}

	return TRUE;
}

int GetJsonBool(cJSON* json, const char* bool_name, bool* output) {
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

void ReadSettings(Settings* settings) {

	if (settings == NULL) {
		return;
	}

	char input[256];
	memset(input, 0, sizeof(input));
	FILE *fp = fopen("sm2000.json", "rb");

	if (fp == NULL) {
		MessageBoxA(NULL, "DUMB", "lol", 0);
		return;
	}

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (fileSize >= sizeof(input)) {
		MessageBoxA(NULL, "Dumb Json", "Resize the json file to be appropriate", 0);
		exit(69);
	}

	fread(input, fileSize, 1, fp);
	cJSON* json = cJSON_ParseWithLength(input, fileSize);

	if (json == NULL) {
		char* error = cJSON_GetErrorPtr();
		error = error == NULL ? "no error" : error;
		MessageBoxA(NULL, error, "Invalid JSON", 0);
		exit(420);
	}

	GetJsonBool(json, "no_videos", &settings->no_videos);
	GetJsonBool(json, "psx_graphics", &settings->psx_graphics);
	GetJsonBool(json, "console", &settings->console);
	GetJsonBool(json, "texture_loader", &settings->texture_loader);
	GetJsonBool(json, "file_loader", &settings->file_loader);
}
