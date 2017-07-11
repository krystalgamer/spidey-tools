#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern FILE *fp;

typedef struct __attribute__((packed)){ 
	uint16_t type; 
	uint32_t size;
	uint16_t reserved1, reserved2;
	uint32_t offBits;
}BmpFileHeader;

typedef struct __attribute__((packed)){
	uint32_t headerSize;
	uint32_t width, height;
	uint16_t planes;
	uint16_t bitCount;
	uint32_t compression;
	uint32_t sizeOfImage;
	uint32_t xpm;
	uint32_t ypm;
	uint32_t clrUsed, clrImp;
	uint32_t redMask, greenMask, blueMask;
	uint32_t alphaMask;
	uint32_t csType;
	uint32_t cie[3*3];
	uint32_t gamma[3];
}BmpImageHeader;

typedef struct __attribute__((packed)){
	BmpFileHeader file;
	BmpImageHeader image; 
}Bmp;

//Open BMP file and returns the RGB565 content
uint16_t *HandleBmpFile(const char *name, uint16_t *width, uint16_t *height){

	if(!name)
		return NULL;

	fp = fopen(name, "rb");
	if(!fp)
		return NULL;

	Bmp curFile;
	if(!fread(&curFile, sizeof(Bmp), 1, fp)){
		fclose(fp);
		return NULL;
	}

	*width = (uint16_t)curFile.image.width;
	*height = -(uint16_t)curFile.image.height;

	//TODO add header check
	
	fseek(fp, curFile.file.offBits, SEEK_SET);
	uint32_t bufferSize = curFile.image.width * -curFile.image.height * 2;
	uint16_t *buffer = malloc(bufferSize);
	if(!buffer){
		fclose(fp);
		return NULL;
	}

	if(!fread(buffer, bufferSize, 1, fp)){
		fclose(fp);
		free(buffer);
		return NULL;
	}

	return buffer;
}
