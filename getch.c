/*
 * getch()
 *
 * Implimention of the above library call.
 *
 * Replaces the broken implimentation of getch() found in the Watcom C/386 9.0
 * libraries. Note: Watcom's ungetch() probably won't work with my new getch().
 *
 * Copyright July 1992 by G. R. (Fred) Walter
 */

#include <stdio.h>
#include <stdlib.h>
#define INCL_KBD
#include <os2.h>

static char        _getch_flag = 0; 
static KBDKEYINFO *_getch_keyinfo = NULL;

int
getch()
{
	if (_getch_keyinfo == NULL) {
		_getch_keyinfo = (KBDKEYINFO *)malloc(sizeof(KBDKEYINFO));
		if (_getch_keyinfo == NULL) {
			fprintf(stderr, "Out of memory error\n");
			exit(1);
		}
	}
			
	if (_getch_flag != 0) {
		_getch_flag = 0;
		return (_getch_keyinfo->chScan);
	}

	if (KbdCharIn(_getch_keyinfo, IO_WAIT, 0) != 0)
		return (EOF);

	if (_getch_keyinfo->chChar == 0x00 || _getch_keyinfo->chChar == 0xE0) {
		_getch_keyinfo->chChar = 0;
		_getch_flag = 1;
	}

	return (_getch_keyinfo->chChar);
}

