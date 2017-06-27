#include "pkr.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include <zlib.h>

static FILE *out = NULL;
static uint8_t buffer[0xFF];

bool ExtractDir(PKRDir *curDir){

	//Set the path
	strcpy(buffer, "extracted\\\0");
	strcat(buffer, (char*)curDir);
	buffer[strlen("extracted\\") + strlen((char*)curDir)] = '\0';

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

	return true;
}
