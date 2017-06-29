#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define FILE_COMPRESSED 0x00000002
#define FILE_UNCOMPRESSED 0xFFFFFFFE

typedef struct{
	uint32_t magic;
	uint32_t dirOffset;
}PKR3File;

typedef struct{
	uint32_t unk;
	uint32_t numDirs;
	uint32_t numFiles;
}PKRDirHeader;

typedef struct{
	char name[0x20];
	uint32_t unk;
	uint32_t numFiles;
}PKRDir;

typedef struct{
	union{
		uint8_t total[0x34];
		struct{
			char name[0x20];
			uint32_t crc;
			uint32_t compressed;
			uint32_t fileOffset;
			uint32_t uncompressedSize;
			uint32_t compressedSize;
		};
	};
}PKRFile;

//walk.c
bool SetupPkrDirs(PKRDir **pkrDirs);
void ExtractDirs(PKRDir *pkrDirs);

//extracted.c
bool ExtractDir(PKRDir *curDir);
