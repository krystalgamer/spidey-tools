#include "pkr.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include <zlib.h>

#define EXTRACT_BUF_SIZE 0xFFFF 

bool ExtractUncompressed(PKRFile *file);
bool ExtractCompressed(PKRFile *file);

extern FILE *fp;
static FILE *out = NULL;
static uint8_t buffer[0xFF];
static uint8_t extractBuffer[EXTRACT_BUF_SIZE];

uint32_t GetFile(PKRFile *file){
	return fread(file, sizeof(PKRFile), 1, fp);
}

bool ExtractDir(PKRDir *curDir){

	//Set the path
	strcpy(buffer, "extracted\\\0");
	strncat(buffer, (char*)curDir, 0x20);

	uint32_t pathLen = strlen("extracted\\") + strlen((char*)curDir);
	buffer[pathLen] = '\0';

	printf("Extracting %s\n", &curDir->name);

	if(mkdir(buffer) && errno != EEXIST){
			if(errno != ENOENT){
				printf("An error occurred while creating the extractee dir: %s(%08X)\n", 
					strerror(errno), errno);

				return false;
			}

			puts("extracted doesn't seem to be yet created.. Creating ");
			//extracted dir doesnt exist so create it
			if(mkdir("extracted")){
				puts("Fail");
				return false;
			}
			puts("Success");
	}

	//Time to get the files
	PKRFile extracted;
	for(uint32_t curFile = 0; curFile<curDir->numFiles; curFile++){

		if(!GetFile(&extracted)){
			printf("Error reading file..");
			return false;
		}

		switch(extracted.compressed){
			case FILE_COMPRESSED:
				if(!ExtractCompressed(&extracted))
					return false;
				break;
			case FILE_UNCOMPRESSED:
				if(!ExtractUncompressed(&extracted))
					return false;
				break;
			default:
				puts("Unknown compression type.. Quitting");
				return false;
		}
			
		//Since path is changed on extraction calls
		buffer[pathLen] = '\0';
	}


	return true;
}

bool ExtractUncompressed(PKRFile *file){
	
	static uint8_t *auxExtractBuf = NULL;
	uint8_t curExtBuf = NULL;//Indicates which buffer is being used for extraction
		

	//Save original offset so it can keep reading the files
	uint32_t originalFp = ftell(fp);
	if(originalFp == 0xFFFFFFFF)	{
		puts("Error getting file position");
		return false;
	}

	if(fseek(fp, file->fileOffset, SEEK_SET)){
		printf("Could not access file %s offset: %08X", file->name, file->fileOffset); 
		return false;
	}

	//Extract part
	buffer[strlen(buffer) + strlen(file->name)] = '\0'
	strncat(buffer, file->name, 0x20);
	out = fopen(buffer, "wb");

	if(!out){
		printf("Could not create the extracted file %s\n", file->name);
		return false;
	}

	if(file->uncompressedSize > EXTRACT_BUF_SIZE){
		auxExtractBuf = malloc(file->uncompressedSize);
		if(!auxExtractBuf){
			printf("Couldn't create extract buffer for %s\n", file->name);
			return false;
		}
		curExtBuf = auxExtractBuf;
	}
	else
		curExtBuf = extractBuffer;
	
	//Should i make this a function??
		
	//Rewind
	if(fseek(fp, originalFp, SEEK_SET)){
		puts("Could not rewind to original offset..");
		return false;
	}

	return true;
}

bool ExtractCompressed(PKRFile *file){
	return true;
}


