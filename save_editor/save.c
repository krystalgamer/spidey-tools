#include <stdio.h> 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "levels.h"


#define IsDigit(x) ('0' <= x && x <= '9')
#define MAX_LEVEL 8
const uint8_t areas[8] = {4, 2, 5, 1, 7, 4, 5, 6};

#define TOTAL_LEVELS (areas[0]+areas[1]+areas[2]+areas[3]+areas[4]+areas[5]+areas[6]+areas[7])
FILE *fp;

typedef struct __attribute__((packed)){

	uint8_t unk[0x204];
	
	//Size BC
	struct _info{ 
		uint32_t checksum;
		char level[7]; 
		uint8_t unk1[0x34];
		char name[9];
		uint8_t unk2[0xD];
		bool playAgain;
		bool levels[35];
		uint32_t curCostume;
		uint32_t unlockedCostumes;
		uint8_t unk3[0x38];
	}info;

}SaveFile;

void CalculateChecksum(SaveFile *save){

	//Start after the checksum
	uint32_t *data = (uint32_t*)&(save->info.level[0]);

	save->info.checksum = 0;
	for(int i = 0; i < 0x2E; i++){
		if(save->info.checksum & 0x80000000)
			save->info.checksum += (save->info.checksum + 1);
		else
			save->info.checksum <<= 1;
		save->info.checksum += data[i];
	}
	save->info.checksum |= 1;
	puts("Checksum calculated!");
}

SaveFile *OpenSaveFile(const char *fileName){

	SaveFile *save = malloc(sizeof(SaveFile));
	if(!save)
		return NULL;

	//Might perform some changes
	fp = fopen(fileName, "rb+");
	if(!fp){
		printf("Error opening %s\n", fileName);
		free(save);
		return NULL;
	}

	if((fread(save, sizeof(SaveFile), 1, fp)) != 1){
		printf("Error reading %s\n", fileName);
		free(save);
		return NULL;
	}

	return save;
}

void WriteChangesToFile(SaveFile *save){
	printf("Writting changes to file.. ");

	//Go to start and write everything
	fseek(fp, 0, SEEK_SET);
	if((fwrite(save, sizeof(SaveFile), 1, fp)) != 1){
		puts("Failed.");
		return;
	}
	puts("Success!");
	fflush(fp);
}

void PrintLevelIds(){
	uint8_t curId = 0;
	for(int i = 0; i < MAX_LEVEL; i++){
		for(int j = 0; j<areas[i]; j++){
			printf("%02d: %d-%d - %s\n", curId++, i+1, j+1, levels[i][j]);
		}
	}
}

void FinishGame(SaveFile* save){
	
	//Set every level has done and activate the flag
	save->info.playAgain = true;
	uint8_t curId = 0;
	for(int i = 0; i<8; i++){
		for(int j = 0; j<areas[i]; j++){
			save->info.levels[curId++] = true;
		}
	}
	puts("All levels cleared!");
	CalculateChecksum(save);
	WriteChangesToFile(save);
	
}

void ModifyCurrentLevel(SaveFile *save, uint32_t chosenLevel){

	uint32_t curLevel = 0;
	uint32_t newLevel = 0, newArea = 0;

	//Sets the levels before the chosen as complete
	for(newLevel = 0; newLevel < MAX_LEVEL; newLevel++){
		for(newArea = 0; newArea<areas[newLevel]; newArea++){
			if(!chosenLevel--)
				goto lock;
			save->info.levels[curLevel++] = true;
		}
	}

	lock:
	//Locks the levels after the chosen
	while(curLevel < TOTAL_LEVELS)
		save->info.levels[curLevel++] = false;
	
	sprintf(save->info.level, "l%1da%1d_t", newLevel+1, newArea+1);
	save->info.playAgain = false;
}

void ChangeLevel(SaveFile *save){
	
	//Get the info
	uint32_t level = atoi(&save->info.level[1]);
	uint32_t area = atoi(&save->info.level[3]);

	//Makes sure everything is ok
	if(level > MAX_LEVEL){
		printf("Unknown level %d\n", level);
		return;
	}
	if(areas[level] < area){
		printf("Unknown area: %d\n", area);
	}
	
	printf("\n\nCurrent level is: %d-%d %s\n", level, area, (save->info.playAgain ? "PLAY AGAIN!" : levels[level-1][area-1]));
	

	bool isDone = false;
	char answer[10] = "c"; 

	fflush(stdin);
	while(!isDone){
		printf("Write the level id to change it\nWrite l to see the level id\nWrite f to finish the game\nWrite q to quit\nOption: ");
	
		if(fgets(answer, 10, stdin) == NULL)
			break;
		
		if(IsDigit(*answer)){
			//Change to the correct id and update the file
			ModifyCurrentLevel(save, atoi(answer));
			CalculateChecksum(save);
			WriteChangesToFile(save);
		}
		else if(*answer == 'l')
			PrintLevelIds();
		else if(*answer == 'f')
			FinishGame(save);
		else if(*answer == 'q')
			isDone = true;

	}
}

void ChangeCurrentCostume(SaveFile *save){

	printf("\n\nCurrent costume is: %s\n\n", costumes[save->info.curCostume]);
	
	//prints costume list
	for(int i = 0; i<10; i++)
		printf("%d - %s\n", i, costumes[i]);
	printf("\n");

	printf("Choose costume: ");
	char choice = 'c';	
	while('0' > choice || choice > '9'){
		choice = getchar();
		fflush(stdin);
	}
	save->info.curCostume = atoi(&choice);
	printf("\nNew costume is: %s\n\n", costumes[save->info.curCostume]);
	CalculateChecksum(save);
	WriteChangesToFile(save);
}

void ChangeUnlockedCostumes(SaveFile* save){

	static const char *lockStatus[2] = {"LOCKED", "UNLOCKED"};
	char answer = 'c';
	while(answer != 'q'){

		printf("\n\n");
		//List unlocked skins
		printf("Write q to quit to go back to menu.\n");
		for(int i = 0;i<10; i++)
			printf("%d - %s (%s)\n", i, costumes[i], lockStatus[((save->info.unlockedCostumes) >> i) & 1]);
		
		printf("Change status of which costume: ");
		answer = getchar();
		fflush(stdin);

		if('0' <= answer && answer <= '9')
			save->info.unlockedCostumes ^= (1 << (atoi(&answer)));
	}
	CalculateChecksum(save);
	WriteChangesToFile(save);
}

void DrawMenu(){

	puts("Spider-Man Save Editor");
	puts("1 - Correct checksum");
	puts("2 - Change Level");
	puts("3 - Change current costume");
	puts("4 - Change unlocked costumes");
	puts("0 - Exit");

}

int main(int argc, char *argv[]){

	//TODO
	//Isolate checksum DONE
	//Open file DONE
	//Read the contents DONE
	//level DONE
	//get current level DONE
	//clamp its values DONE
	//change them DONE
	//Costume change current DONE
	//Change unlocked costumes DONE
	
	setbuf(stdout, NULL);
	if(argc != 2){
		printf("Input type: \n%s savefile\n", argv[0]);
		return 1;
	}

	SaveFile *save = OpenSaveFile(argv[1]);
	if(!save)
		return 2;

	char option = 'c';
	DrawMenu();

	while(option != '0'){
		option = getchar();	
		fflush(stdin);
		switch(option){
			case '0':
				continue;	
			case '1':
				CalculateChecksum(save);
				WriteChangesToFile(save);
				break;
			case '2':
				ChangeLevel(save);
				break;
			case '3':
				ChangeCurrentCostume(save);
				break;
			case '4':
				ChangeUnlockedCostumes(save);
				break;
		}
		printf("\n\n");
		DrawMenu();
	}
	
	fclose(fp);
	return 0;
}
