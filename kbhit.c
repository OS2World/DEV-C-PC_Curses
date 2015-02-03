/*
 * kbhit()
 *
 * Implimention of the above library call.
 *
 * Replaces the broken implimentation of kbhit() found in the Watcom C/386 9.0
 * libraries.
 *
 * Copyright July 1992 by G. R. (Fred) Walter
 */

#include <stdio.h>
#include <stdlib.h>
#define INCL_KBD
#include <os2.h>

static KBDKEYINFO *_kbhit_keyinfo = NULL;

int
kbhit()
{
	if (_kbhit_keyinfo == NULL) {
		_kbhit_keyinfo = (KBDKEYINFO *)malloc(sizeof(KBDKEYINFO));
		if (_kbhit_keyinfo == NULL) {
			fprintf(stderr, "Out of memory error\n");
			exit(1);
		}
	}
			
	if (KbdPeek(_kbhit_keyinfo, 0) != 0)
		return (0);

	if (_kbhit_keyinfo->fbStatus & 0x40)
		return (1);
	else
		return (0);
}

