#include "pkr.h"

FILE *fp = NULL;
bool ignore_crc = false;

int main(int argc, char *argv[]){

    const char *filename = NULL;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "--ignore-crc") == 0){
            ignore_crc = true;
        }
        else if(!filename){
            filename = argv[i];
        }
        else{
            puts("Too many arguments");
            return 1;
        }
    }

    if(!filename){
        puts("Please specify file name");
        return 1;
    }

    fp = fopen(filename, "rb");

    if(!fp){
        printf("Couldnt open file %s\n", filename);
        return 2;
    }

    PKRDir *pkrDirs = NULL;
    if(SetupPkrDirs(&pkrDirs))
        ExtractDirs(pkrDirs);

    fclose(fp);
    return 0;
}
