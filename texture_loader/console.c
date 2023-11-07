#include <windows.h>
#include <stdio.h>
#include "console.h"

static int IsAllocated = 0;

int AllocateConsole(void) {

	if (IsAllocated){
		return 0;
	}

	AllocConsole();

	if(freopen("CONOUT$", "w", stdout)){
		IsAllocated = 1;
	}
	else {
		return 1;
	}

	return 0;
}

void ReleaseConsole(void) {
	if (!IsAllocated) {
		return;
	}

	FreeConsole();
}