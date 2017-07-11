#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "texture.h"


bool AddTextureToPsx(uint8_t curTex, uint8_t *colorPal, uint8_t *offList, uint32_t maxOff);
void ClosePsxFile();
bool SetupPsxFile(const char *psxName);

FILE *fp = NULL;
uint32_t *bruijn = (uint32_t*)_bruijn;
bool SetupColors();
static uint8_t colorPal[0x800];

uint16_t *HandleBmpFile(const char *name, uint16_t *width, uint16_t *height);

typedef struct _offset{
	uint32_t offset;
	struct _offset *next;	
}OffsetList;

typedef struct _color{
	uint16_t val[4];
	OffsetList *list;	
	struct _color *next;
}Color;

bool ExportPSVR(const char *psxFile, uint8_t curTex, Color *list);

Color *CreateColor(){
	Color *tmp = malloc(sizeof(Color));
	if(!tmp)
		return NULL;
	memset(tmp, 0, sizeof(Color));
	return tmp;
}

bool AddToOffsetList(OffsetList **list, uint32_t offset){
	
	OffsetList **curOffset = list;
	while(*curOffset)
		curOffset = &((*curOffset)->next);

	*curOffset = malloc(sizeof(OffsetList));
	if(!(*curOffset))
		return false;
	memset(*curOffset, 0, sizeof(OffsetList));

	(*curOffset)->offset = offset;
	return true;
}

bool AlreadyAdded(Color *cur, Color **list, uint32_t offset){
	//Not created the list yet
	if(!(*list))
		return false;
	
	Color *entry = *list;
	while(entry){
		if((*cur).val[0] != (*entry).val[0])
			goto next;
		if((*cur).val[1] != (*entry).val[1])
			goto next;
		if((*cur).val[2] != (*entry).val[2])
			goto next;
		if((*cur).val[3] != (*entry).val[3])
			goto next;
		
		//Just add the offset to the list
		if(!AddToOffsetList(&entry->list, offset))
			return false;

		return true;
		next:
		entry = entry->next;
	}
	return false;
}

bool AddToColorList(Color *cur, Color **list, uint32_t offset){
	
	//Creates new color
	Color *tmp = CreateColor();
	if(!tmp)
		return false;
	
	memcpy(tmp, cur, 2*4);

	//Creates it offset list
	if(!AddToOffsetList(&tmp->list, offset))
		return false;

	//FIFO
	Color **lastEntry = list;
	while(*lastEntry)
		lastEntry = &((*lastEntry)->next);

	*lastEntry = tmp;
	return true;
}

uint32_t GetPatterns(const char *psxFile, uint8_t curTex, uint16_t *buffer, uint16_t width, uint16_t height){

	uint16_t actualWidth = width >> 1;
	uint16_t actualHeight = height >> 1;

	if(actualWidth >= actualHeight)
		actualWidth = height >> 1;

	uint32_t v20 = actualWidth - 1;
	uint32_t v32 = 0;
	if(v20 & 1)
		for(uint32_t i = 1; i & v20; i*=2, v32++);

	uint32_t curWidth, curHeight;
	curWidth = curHeight = 0;
	uint32_t v30 = 0;
	Color curColors;
	Color *list = NULL;
	uint32_t numColors = 0;
	uint32_t oldV30 = 0;

	do{
		curWidth = 0;
		if(width >> 1){
			do{
				v30 = bruijn[v20 & curHeight]
					| 2 * bruijn[v20 & curWidth] | ((~v20 & (curHeight | curWidth)) << v32);

				curColors.val[0] = buffer[curHeight * width * 2 + curWidth * 2];
				curColors.val[2] = buffer[curHeight * width * 2 + curWidth * 2 + 1];
				curColors.val[1] = buffer[width + curHeight * width * 2 + curWidth * 2];
				curColors.val[3] = buffer[width + curHeight * width * 2 + curWidth * 2 + 1];
				
				/*printf("%04X %04X %04X %04X\n", curColors.val[0],curColors.val[1],curColors.val[2],
						curColors.val[3]);*/

				if(!AlreadyAdded(&curColors, &list, v30)){
					if(!AddToColorList(&curColors, &list, v30))
						return 0;
					numColors++;
				}
				curWidth++;
			}while(curWidth < (width >> 1));
		}
		curHeight++;
	}while(curHeight < (height >> 1));

	if(numColors > 256){
		printf("Excess: %d\n", numColors);
		return 0;
	}

	if(!ExportPSVR(psxFile, curTex, list)){
		return 0;
	}
	return numColors;
}

bool ExportPSVR(const char *psxFile, uint8_t curTex, Color *list){
	
	//Find highest offset
	Color *curColor = list;
	uint32_t maxOff = 0;
	while(curColor){
		OffsetList *offList = curColor->list;
		while(offList){
			if(maxOff<offList->offset)
				maxOff = offList->offset;
			offList = offList->next;
		}
		curColor = curColor->next;
	}
	uint8_t *afterColor = malloc(maxOff+1);
	if(!afterColor)
		return false;

	//Define offset list
	//And paint the colors
	curColor = list;
	Color *oldColor = NULL;
	OffsetList *oldOff = NULL;
	uint32_t i = 0;
	while(curColor){
		memcpy(&colorPal[i*8], curColor, 2*4);

		OffsetList *offList = curColor->list;
		while(offList){
			afterColor[offList->offset] = i;
			oldOff = offList;
			offList = offList->next;
			free(oldOff);
		}
		i++;
		oldColor = curColor;
		curColor = curColor->next;
		free(oldColor);
	}
	if(!SetupPsxFile(psxFile)){
		free(afterColor);
		return false;
	}
		
	if(!AddTextureToPsx(curTex, colorPal, afterColor, maxOff+1)){
		free(afterColor);
		return false;
	}
	ClosePsxFile();

	return true;
}

int main(int argc, char *argv[]){
	
	if(argc != 3)
		return 1;
	printf("Size %d\n", sizeof(Color));
	
	uint16_t width;
	uint16_t height;

	uint16_t *textureBuffer = HandleBmpFile(argv[1], &width, &height);
	if(!textureBuffer)
		return 2;
	
	printf("Texture uses: %d patterns", GetPatterns(argv[2], atoi(argv[1]), textureBuffer, width, height));
}
