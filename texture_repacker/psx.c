#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//Copy pasted from my psx_extractor

FILE *psx = NULL;
uint32_t numTextures = 0;
extern uint8_t *afterColor;

typedef struct{
	uint8_t unk[0xC];//Includes the name
	uint32_t add1[3];
	uint32_t buffer;
	uint32_t add2[2];
}Mem;

bool GetAdd1(Mem *mem){
	
	//loc_4C9BAF
	fseek(psx, 4, SEEK_SET);

	if(!fread(&(mem->add1[0]), 4, 1, psx))
		return false;

	if(mem->add1[0] == 0xFFFFFFFF)
		return true;

	//loc_4C9BBF
	uint32_t newAdd = 0;
	do{

		fseek(psx, mem->add1[0]+4, SEEK_SET);
		if(!fread(&newAdd, 4, 1, psx))
			return false;

		mem->add1[0] += newAdd + 8;

		fseek(psx, mem->add1[0], SEEK_SET);
		if(!fread(&newAdd, 4, 1, psx))
			return false;

	}while(newAdd != 0xFFFFFFFF);

	mem->add1[0] += 4;
	return true;
}

bool GetV13(uint32_t *v13){
	
	fseek(psx, 8, SEEK_SET);
	if(!fread(v13, 4, 1, psx)){
		puts("Couldnt read shit on off 8");
		return false;
	}

	fseek(psx, (9 * *v13 + 3) * 4, SEEK_SET);
	if(!fread(v13, 4, 1, psx)){
		puts("Error getting v13");
		return false;
	}
	return true;
}

bool GetV101(uint32_t *v101, Mem *mem, uint32_t v13){

	//add1[0] is v19
	fseek(psx, (v13 * 4) + mem->add1[0], SEEK_SET);
	if(!fread(v101, 4, 1, psx))
		return false;

	return true;
}

void GetV35(uint32_t *v35, uint32_t v13, uint32_t v101, uint32_t v34){

	//v13 = v100
	//&v34[4 * (v100 + v101) + 8]

	*v35 = v34 + ((v13 + v101) * 4 + 8);
}

bool GetNumTextures(uint32_t v41){
	fseek(psx, v41, SEEK_SET);
	if(!fread(&numTextures, 4, 1, psx))
		return false;
	return true;
}

typedef struct{
	uint8_t unk[0x10];
	uint16_t width;
	uint16_t height;
	uint32_t palette;
	uint32_t size;
}PSXPVR;

bool SetupPsxFile(const char *psxName){

	psx = fopen(psxName, "r+b");
	if(!psx){
		puts("Couldnt open the file");
		return false;
	}

	Mem mem;
	memset(&mem, 0, sizeof(mem));
	if(!GetAdd1(&mem))
		return false;

	uint32_t v13 = 0;
	if(!GetV13(&v13))
		return false;

	uint32_t v101 = 0;
	if(!GetV101(&v101, &mem, v13))
		return false;
	
	uint32_t v35 = 0;
	GetV35(&v35, v13, v101, mem.add1[0] - 4);//v34 = add1[0] - 4

	uint32_t v37 = v35 + 4;
	uint32_t v41 = v37 + 4;

	if(!GetNumTextures(v41))
		return false;

	return true;
}

bool AddTextureToPsx(uint8_t curTex, uint8_t *colorPal, uint8_t *offList, uint32_t maxOff){

		fseek(psx, 4 * curTex, SEEK_CUR);
		uint32_t textureOff = 0;
		if(!fread(&textureOff, 4, 1, psx))
			return false;
		fseek(psx, textureOff, SEEK_SET);
		
		PSXPVR pvr;
		if(!fread(&pvr, sizeof(pvr), 1, psx))
			return false;
		
		if(pvr.size < ((maxOff + 1) + 0x800))
			return false;

		rewind(psx);
		fseek(psx, textureOff+sizeof(pvr), SEEK_SET);
		if(!fwrite(colorPal, 0x800, 1, psx))
			return false;
		if(!fwrite(offList, maxOff, 1, psx))
			return false;
		
		return true;
}

void ClosePsxFile(){
	fclose(psx);
}
