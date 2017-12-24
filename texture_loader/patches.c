/*
 * Purpose: Contains all the patches
 * with each call to memory related functions
 * being documented to ease debugging
 *
 */

#include "patches.h"


BOOL DisableIntros(){

	if (!NopMemory(0x004707BE, 0x004707C3 - 0x004707BE, "Disables wrapper for Bink functions."))
		return FALSE;
	if (!NopMemory(0x004707C8, 0x004707CE - 0x004707C8, "Unknown, related to some global var"))
		return FALSE;
	
	return TRUE;
	
}
