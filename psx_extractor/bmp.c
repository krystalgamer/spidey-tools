#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct __attribute__((packed)){ 
	uint16_t type; uint32_t size;
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

bool WriteBmpFile(uint8_t *buffer, uint32_t width, uint32_t height, uint32_t curTexture){
	
	static Bmp extracted;
	memset(&extracted, 0, sizeof(Bmp));

	//Setup fileHeader
	strcpy((char*)&extracted.file.type, "BM");
	extracted.file.size = sizeof(Bmp) + width * height * 2;
	extracted.file.offBits = sizeof(Bmp); 

	//Image header
	extracted.image.headerSize = sizeof(BmpImageHeader);
	extracted.image.width = width;
	extracted.image.height = -height;
	extracted.image.planes = 1;
	extracted.image.bitCount = 16;
	extracted.image.compression = 3;//BI_BITFIELDS
	extracted.image.sizeOfImage = width * height * 2;
	extracted.image.xpm = extracted.image.ypm = 
		extracted.image.clrUsed = extracted.image.clrImp = 0;

	
	extracted.image.redMask = 0xF800;
	extracted.image.blueMask = 0x1F;
	extracted.image.greenMask = 0x07E0;

	char extractedName[32];
	sprintf(extractedName, "%d.bmp", curTexture);

	FILE *fp = fopen(extractedName, "wb");
	if(!fp)
		return false;

	if(!fwrite(&extracted, sizeof(Bmp), 1, fp))
		return false;
	if(!fwrite(buffer, width*height*2, 1, fp))
		return false;

	return true;
	
}
