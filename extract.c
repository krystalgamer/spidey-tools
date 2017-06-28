#include "pkr.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include <zlib.h>

#define EXTRACT_BUF_SIZE 0xFFFF 

//Extract subroutines
bool ExtractUncompressed(PKRFile *file);
bool ExtractCompressed(PKRFile *file);

bool GetFile(PKRFile *file);
bool WriteFileToDisk(PKRFile *file);

//Checksum
bool calculateExtractedCrc(PKRFile *file);

extern FILE *fp;
static FILE *out = NULL;

static uint8_t buffer[0xFF];
static uint8_t extractBuffer[EXTRACT_BUF_SIZE];

static uint8_t *auxExtractBuf = NULL; //Used when extract buffer is not enough
uint8_t *curExtBuf = NULL;//Indicates which buffer is being used for extraction
		
//Gets PKRFile struct
uint32_t GetPkrFile(PKRFile *file){
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

		if(!GetPkrFile(&extracted)){
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
				printf("Unknown compression type:%08X.. Quitting", extracted.compressed);
				return false;
		}
			
		//Since path is changed on extraction calls
		buffer[pathLen] = '\0';
	}


	return true;
}

bool ExtractUncompressed(PKRFile *file){
	if(!GetFile(file))
		return false;

	return WriteFileToDisk(file);
}

bool ExtractCompressed(PKRFile *file){
	return true;
}

//Get file off pkr
bool GetFile(PKRFile *file){
	
	//Save original offset so it can keep reading the files
	uint32_t originalFp = ftell(fp);
	if(originalFp == 0xFFFFFFFF){
		puts("Error getting file position");
		return false;
	}

	if(fseek(fp, file->fileOffset, SEEK_SET)){
		printf("Could not access file %s offset: %08X", file->name, file->fileOffset); 
		return false;
	}
	
	//Get the file size regardless of compression status
	uint32_t fileSize = (file->compressed == FILE_UNCOMPRESSED) ? 
			file->uncompressedSize : file->compressedSize;

	if(fileSize > EXTRACT_BUF_SIZE){

		auxExtractBuf = malloc(fileSize);

		if(!auxExtractBuf){
			printf("Couldn't create extract buffer for %s\n", file->name);
			return false;
		}
		curExtBuf = auxExtractBuf;
	}
	else
		curExtBuf = extractBuffer;

	if(!(fread(curExtBuf, fileSize, 1, fp))){
		printf("Could not read file %s\n", file->name);
		return false;
	}

	//Rewind
	if(fseek(fp, originalFp, SEEK_SET)){
		puts("Could not rewind to original offset..");
		
		if(auxExtractBuf){
			free(auxExtractBuf);
			curExtBuf = auxExtractBuf = NULL;
		}
			
		return false;
	}

	if(!calculateExtractedCrc(file)){
		printf("CRC doesnt match: %s", file->name);
		return false;
	}

	return true;
}

bool WriteFileToDisk(PKRFile *file){
	
	buffer[strlen(buffer) + strlen(file->name)] = '\0';
	strncat(buffer, file->name, 0x20);
	out = fopen(buffer, "wb");

	if(!out){
		printf("Could not create the extracted file %s\n", file->name);
		return false;
	}
	
	if(!(fwrite(curExtBuf, file->uncompressedSize, 1, out))){
		puts("Could not write file to disk");

		if(auxExtractBuf){
			free(auxExtractBuf);
			curExtBuf = auxExtractBuf = NULL;
		}

		return false;
	}

	fclose(out);
	
	//Clear the auxiliary buffer
	if(auxExtractBuf){
		free(auxExtractBuf);
		curExtBuf = auxExtractBuf = NULL;
	}

	return true;
}

bool calculateExtractedCrc(PKRFile *file){
	uint32_t crc = 0; 

	crc = crc32(crc, curExtBuf, file->uncompressedSize);
	
	return (crc == file->crc);
}
