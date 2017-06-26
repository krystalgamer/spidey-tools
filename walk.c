//Related to walking the PKR3 Files directories
//

#include "pkr.h"

extern FILE *fp;

const char *magic = "PKR3";
static PKRDirHeader pkrDirHeader;

bool GetFileHeader(PKR3File *pkr){
	
	if(fread(pkr, sizeof(PKR3File), 1, fp) != 1){
		puts("Error reading the file.");
		return false;
	}

	if((memcmp(&(pkr->magic), magic, 4))){
		puts("Invalid PKR3 Header.");
		return false;
	}
	return true;
}

//Gets the header of the directories
bool GetPkrDirsHeader(PKR3File *pkr){

	//Go to dir offset
	fseek(fp, pkr->dirOffset, SEEK_SET);

	memset(&pkrDirHeader, 0, sizeof(PKRDirHeader));

	if(fread(&pkrDirHeader, sizeof(PKRDirHeader), 1, fp) != 1){
		puts("Couldn't get the dirs of PKR");
		return false;
	}
	
	printf("There are %d dirs and %d files\n", pkrDirHeader.numDirs, pkrDirHeader.numFiles);
	
	return true;
}

//Loads the info about the dirs
bool LoadDirectories(PKRDir *pkrDirs){
	
	uint32_t dirsRead = 0;
	dirsRead = fread(pkrDirs, sizeof(PKRDir), pkrDirHeader.numDirs, fp);

	if(dirsRead != pkrDirHeader.numDirs){
		printf("Could only read %d dirs.", dirsRead);
		return false;
	}

	for(int i = 0; i<dirsRead; i++)
		printf("%s has %d files\n", &pkrDirs[i].name, pkrDirs[i].numFiles);

	return true;
}

bool LoadFiles(PKRFile *buffer, uint32_t numFiles){
	return (fread(buffer, sizeof(PKRFile), numFiles, fp) == numFiles);
}

//Prints the name of all the files in a dir
void ListDirFiles(PKRFile *files, uint32_t numFiles){
	for(uint32_t i = 0; i<numFiles; i++)
		printf("\t%s\n", &files[i]);
}

bool SetupPkrDirs(PKR3File *pkr, PKRDir **pkrDirs){

	//Checks if file is valid
	if(!GetFileHeader(pkr))
		return false;

	//Loads num dirs and total num files
	if(!GetPkrDirsHeader(pkr))
		return false;

	*pkrDirs = malloc(sizeof(PKRDir) * pkrDirHeader.numDirs);
	if(!(*pkrDirs)){
		puts("Couldnt allocate space for the dirs");
		return false;
	}

	if(!LoadDirectories(*pkrDirs)){
		free(*pkrDirs);
		return false;
	}

	return true;		
}

void ExtractDirs(PKRDir *pkrDirs){
	
	PKRFile file;
	for(uint32_t curDir = 0; curDir<pkrDirHeader.numDirs; curDir++){
		printf("Extracting %s\n", pkrDirs[curDir]);

		for(uint32_t curFile = 0; curFile<pkrDirs[curDir].numFiles; curFile++){
			
		}
	}
}
