#include <stdio.h.>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

FILE *fp = NULL;
uint8_t buffer[256];
uint8_t *trgBuffer = NULL;

bool OpenTrg(char *filename){
	fp = fopen(filename, "rb");
	return (fp != NULL);
}

bool IsValidTrg(){
	//Check magic number
	if(!fread(buffer, 4, 1, fp))
		return false;
	if(strncmp("_TRG", buffer, 4)){
		puts("Invalid TRG header");
		return false;
	}

	if(!fread(buffer, 4, 1, fp))
		return false;
	if(*(uint32_t*)&buffer[0] & 0xFFFF == 2){
		puts("Wrong TRG version");
		return false;
	}
	if(*(uint32_t*)&buffer[0] & 0xFFFF0000 == 0x10000){
		puts("Not a spiderman trigger file");
		return false;
	}

	return true;
}

bool InitTrg(){

	fseek(fp, 0, SEEK_END);
	uint32_t fileSize = ftell(fp);

	trgBuffer = malloc(fileSize);
	if(!trgBuffer)
		return false;//Failed to allocate

	fseek(fp, 0, SEEK_SET);
	if(!fread(trgBuffer, fileSize, 1, fp)){
		puts("Couldn't read contents of TRG file...");
		return false;
	}

	return true;
}

int main(int argc, char **argv){

	if(argc != 2){
		printf("Format is:\n%s filename",argv[0]);
		return 1;
	}

	if(!OpenTrg(argv[1])){
		printf("Couldn't open %s\n", argv[1]);
		return 2;
	}

	if(!IsValidTrg())
		return 3;
	if(!InitTrg())
		return 4;

	puts("TRG successfully initialized");

	return 0;
}
