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
            
					FILE LOADER
            
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

		printf("Will open from the pkr: %s\n", fileName);
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

	printf("File has been loaded: %s\n", curPkr->name);
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
/************************************************
            
				TEXTURE LOADER
            
 ************************************************/

void* PVRIdHandler(DWORD id, DWORD width, DWORD height, DWORD type, DWORD texturebuffer, void *a6, void *a7, void *a8);
void* CreateTexturePVRInIdHook();
void* ConvertVQHook();

typedef void* (*createTexturePVRInIdP)(DWORD id, DWORD, DWORD , DWORD type, DWORD textureBUffer, void *a6, void *a7, void *a8);
createTexturePVRInIdP CreateTexturePVRInId = (void*)0x0050F6D0;

DWORD *UnkList = (void*)0x6B2440;
DWORD psxId = 0;
void* textures[1024];

BOOL TextureLoader(){

	memset(textures, 0, sizeof(textures));
	Hook(0x0050F45F, (DWORD)CreateTexturePVRInIdHook, "Hooks a call to CreateTexturePVRInIdHook for the PSXPVR files")
	Hook(0x0050F951, (DWORD)ConvertVQHook, "Hooks ConvertVQToBmp")
	return TRUE;

}

void *PVRIdHandler(DWORD id, DWORD width, DWORD height, DWORD type, DWORD textureBuffer, void *a6, void *a7, void *a8){


	void* res = NULL;
	static char path[64];
	sprintf(path, "textures/%s/%08X.bmp\0", &UnkList[0x11 * psxId], textureBuffer - UnkList[0x11 * psxId + 6]);

	FILE *fp;
	fp = fopen(path, "rb");
	if(!fp)
		goto load_texture_fail;

	//Guarantees there's no problems with the files
	DWORD textureSize = MyGetFileSize(fp);
	if(textureSize < width * height * 2 + 0xA){
		printf("There seems to be a problem with the texture %08X for %s\n", textureBuffer - UnkList[0x11 * psxId + 6], &UnkList[0x11 * psxId]);
		fclose(fp);
		goto load_texture_fail;
	}

	//opens the bmp and reads the pixel info
	DWORD offset;
	fseek(fp, 0xA, SEEK_SET);
	if(!fread(&offset, 4, 1, fp)){
		fclose(fp);
		printf("Couldn't read offset %s\n", &UnkList[0x11 * psxId]);
		goto load_texture_fail;
	}

	fseek(fp, offset, SEEK_SET);
	void *texBuffer = new(width * height * 2);	
	if(!texBuffer){
		fclose(fp);
		printf("Couldn't allocate space for %s\n", &UnkList[0x11 * psxId]);
		goto load_texture_fail;
	}

	if(!fread(texBuffer, width * height * 2, 1, fp)){
		fclose(fp);
		delete(texBuffer);
		printf("Couldn't read offset %s\n", &UnkList[0x11 * psxId]);
		goto load_texture_fail;
	}

	fclose(fp);//Dont leak file pointers
	res = texBuffer;
	printf("Successfully loaded a texture! %s %08X\n", path, res);

load_texture_fail:
	if (textures[id] != NULL && res == NULL) {
		delete(textures[id]);
	}
	textures[id] = res;
	return CreateTexturePVRInId(id, width, height, type, textureBuffer, a6, a7, a8);
}

void* get_texture(int id, int width, int height) {
	if (id >= 1024)
		return NULL;

	void* cur = textures[id];
	if (!cur)
		return NULL;

	void* res = new(width * height * 2);
	if (!res)
		return NULL;
	memcpy(res, cur, width * height * 2);
	return res;
}

/************************************************
            
					OPTIONS

************************************************/
 
void addMenuEntryRedirect();
const char *option = "Mod configuration";
DWORD optionAdd = (DWORD)&option;

BOOL ModOptions(){

	Hook(0x0049716C, (DWORD)addMenuEntryRedirect, "Hooking the last addMenuEntry of options to add mine")
	return TRUE;
}

/************************************************
            
					LOW RES (aka PSX MODE)

************************************************/

static int dummyRead = 0;
BOOL LowRes() {

	DWORD* low_graphics = (DWORD*)0x006B78F8;
	*low_graphics = 1;

	DWORD dummyAddress = (DWORD)&dummyRead;
	SetMemory(0x005156EC, 4, &dummyAddress, "Override address of read low_graphics");

	/*
	* Removed this because it breaks config
	DWORD freadSkip = 0x005156F0;
	Nop(freadSkip, 5, "Remove fread from Spidey.cfg of low_graphics");
	*/

	DWORD freadFirst = 0x005156FF;
	Nop(freadFirst, 6, "Remove low_graphics override after reading config");


	DWORD setDisplay = 0x0050034A;
	Nop(setDisplay, 6, "Remove low_graphics override in top of setDisplayOptions");
	return TRUE;
}
