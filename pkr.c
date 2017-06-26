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

	PKR3File pkr = {0, 0};
	PKRDir *pkrDirs = NULL;
	if(!SetupPkrDirs(&pkr, &pkrDirs))
		goto error;
	/*
	//PKR3File(container) != PKRFile(file inside)
	PKRFile **files = NULL;
	files = malloc(sizeof(PKRFile*) * pkrDirHeader.numDirs);
	if(!files){
		printf("Couldnt create dir tree");
		goto error1;
	}

	for(int i = 0; i<pkrDirHeader.numDirs; i++){
		files[i] = malloc(pkrDirs[i].numFiles * sizeof(PKRFile));
		if(!files[i]){
			printf("Couldnt get space for the files");
			goto error1;
		}
		if(LoadFiles(files[i], pkrDirs[i].numFiles))
			printf("%s successfuly read.\n", pkrDirs[i].name);
		//Commented due to only being used for testing purposes
		//ListDirFiles(files[i], pkrDirs[i].numFiles);
		
	}
	*/

	error:
	fclose(fp);
	return 0;
}
