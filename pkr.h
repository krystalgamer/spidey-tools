#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


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
		//TODO implement rest of struct
		struct{
			char name[20];
			
		};
	};
}PKRFile;

//walk.c
bool SetupPkrDirs(PKR3File *pkr, PKRDir **pkrDirs);
