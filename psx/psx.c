#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>


typedef struct{
	uint8_t unk[0xC];//Includes the name
	uint32_t add1[3];
	uint32_t buffer;
	uint32_t add2[2];
}Mem;

FILE *fp = NULL;

bool GetAdd1(Mem *mem){
	
	//loc_4C9BAF
	fseek(fp, 4, SEEK_SET);

	if(!fread(&(mem->add1[0]), 4, 1, fp))
		return false;

	if(mem->add1[0] == 0xFFFFFFFF)
		return true;

	//loc_4C9BBF
	uint32_t newAdd = 0;
	do{

		fseek(fp, mem->add1[0]+4, SEEK_SET);
		if(!fread(&newAdd, 4, 1, fp))
			return false;

		mem->add1[0] += 4;
		mem->add1[0] += newAdd + 4;

		fseek(fp, mem->add1[0], SEEK_SET);
		if(!fread(&newAdd, 4, 1, fp))
			return false;

	}while(newAdd != 0xFFFFFFFF);

	mem->add1[0] += 4;
	return true;
}

bool GetV13(uint32_t *v13){
	
	fseek(fp, 8, SEEK_SET);
	if(!fread(v13, 4, 1, fp)){
		puts("Couldnt read shit on off 8");
		return false;
	}

	fseek(fp, (9 * *v13 + 3) * 4, SEEK_SET);
	if(!fread(v13, 4, 1, fp)){
		puts("Error getting v13");
		return false;
	}
	return true;
}

bool GetV101(uint32_t *v101, Mem *mem, uint32_t v13){

	//add1[0] is v19
	fseek(fp, (v13 * 4) + mem->add1[0], SEEK_SET);
	if(!fread(v101, 4, 1, fp))
		return false;

	return true;
}

void GetV35(uint32_t *v35, uint32_t v13, uint32_t v101, uint32_t v34){

	//v13 = v100
	//&v34[4 * (v100 + v101) + 8]

	*v35 = v34 + ((v13 + v101) * 4 + 8);
}

bool GetNumTextures(uint32_t *numTextures, uint32_t v41){
	fseek(fp, v41, SEEK_SET);
	if(!fread(numTextures, 4, 1, fp))
		return false;
	return true;
}

void GetTexturesAdd(uint32_t numTextures){
	uint32_t tmp = 0;
	for(int i=0; i<numTextures; i++){
		if(!fread(&tmp, 4, 1, fp))
			return;
		printf("%d: %08X hkjh\n", i+1, tmp);
	}
}

uint32_t colors[0x400];

bool SetupColorStuff(){
	memset(colors, 0, 0x400 * 4);

	for(uint32_t counter = 0; counter<0x400; counter++){
	
		//unk1 eax
		//unk2 edx
		//unk3 esi
		//unk4 ecx
		//loc_5115EB
		uint32_t unk1 = (counter >> 1) & 0x55555555;
		uint32_t unk2 = unk1 & 0x33333333;
		uint32_t unk3 = (unk1 * 2) ^ counter;
		uint32_t unk4 = ((unk3 >> 2) & 0x33333333) ^ unk2;

		unk1 ^= unk4;
		unk4 <<= 2;
		unk3 ^= unk4;

		unk2 = unk1 & 0x0F0F0F0F;
		unk4 = (unk3 >> 4) & 0x0F0F0F0F;
		unk4 ^= unk2;
		unk1 ^= unk4;
		unk4 <<= 4;
		unk3 ^= unk4;
		
		unk2 = (unk1 & 0x00FF00FF);
		unk4 = (unk3 >> 8) & 0x00FF00FF;
		unk3 &= 0xFFFF;
		unk4 ^= unk2;

		unk2 = 0;
		unk2 |= (unk4 & 0xFF) << 8;
		
		unk4 ^= unk1;
		unk2 ^= unk3;
		unk4 <<= 0x10;
		unk2 |= unk4;
		
		colors[counter] = unk2;	
		
	}

	return true;
}

void PrintColors(){
	for(uint32_t i = 0; i<0x400; i++)
		printf("%08X ", colors[i]);
}

typedef struct{
};
int main(int argc, char *argv[]){

	if(argc != 2)
		return 1;

	fp = fopen(argv[1], "rb");
	if(!fp){
		puts("Couldnt open the file");
		return 2;
	}

	Mem mem;
	memset(&mem, 0, sizeof(mem));
	if(!GetAdd1(&mem))
		return 3;

	uint32_t v13 = 0;
	if(!GetV13(&v13))
		return 4;

	uint32_t v101 = 0;
	if(!GetV101(&v101, &mem, v13))
		return 5;
	
	uint32_t v35 = 0;
	GetV35(&v35, v13, v101, mem.add1[0] - 4);//v34 = add1[0] - 4

	uint32_t v37 = v35 + 4;
	uint32_t v41 = v37 + 4;

	uint32_t numTextures = 0;
	if(!GetNumTextures(&numTextures, v41))
		return 6;

	//fp is now currently at the first texture add
	printf("ADD1 %08X %08X %08X \nv13:%08X v101:%08X v35:%08X"
			"\nv41:%08X\n",
		   	mem.add1[0], mem.add1[1],mem.add1[2], v13, v101, v35, v41);
	
	printf("There are %d textures.\n", numTextures);
	GetTexturesAdd(numTextures);

	if(!SetupColorStuff())	
		return 7;

	return 0;
}
