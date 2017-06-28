#include "pkr.h"

FILE *fp = NULL;

int main(int argc, char *argv[]){

	if(argc != 2){
		puts("Please specify file name");
		return 1;
	}

	fp = fopen(argv[1], "rb");

	if(!fp){
		printf("Couldnt open file %s\n", argv[1]);
		return 2;
	}

	PKRDir *pkrDirs = NULL;
	if(!SetupPkrDirs(&pkrDirs))
		goto error;
	
	ExtractDirs(pkrDirs);	

	error:
	fclose(fp);
	return 0;
}
