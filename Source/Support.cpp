#include "Support.h"
#include "Files.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>                     
#include <unistd.h>

#include "SDL.h"

void Microseconds(UnsignedWide *microTickCount)
{
	/* NOTE: hi isn't used in BS, so it's not implemented here */
	/* TODO: does game need microsecond precision? */
	microTickCount->hi = 0;
	microTickCount->lo = SDL_GetTicks() * 1000;
}

void GetMouse(Point *p)
{
STUB_FUNCTION;
}

void GetKeys(unsigned long *keys)
{
STUB_FUNCTION;
}

int Button(void)
{
STUB_FUNCTION;
	return 0;
}

void InitMouse()
{
STUB_FUNCTION;
}

void MoveMouse(int xcoord, int ycoord, Point *mouseloc)
{
STUB_FUNCTION;
}

void DisposeMouse()
{
STUB_FUNCTION;
}

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

static void fix_filename(const char *original, char *fixed)
{
	const char *start;
	int i;
	int len;

	start = original;
	if (original[0] == ':') {
		start = &original[1];
	}
	
	/* 
	  here would be stuff to check where the file is,
	  including the game root and the user local dir
	 */
	
	fixed[MAX_PATH-1] = 0;
	
	strncpy(fixed, start, MAX_PATH);
	
	/* check to see if strncpy overwrote the terminator */
	if (fixed[MAX_PATH-1] != 0) {
		fixed[MAX_PATH-1] = 0;
	
		fprintf(stderr, "ERROR: file truncation error: %s -> %s\n", 
			original, fixed);
	}
	
	len = strlen(fixed);
	for (i = 0; i < len; i++) {
		if (fixed[i] == ':') {
			fixed[i] = '/';
		}
	}
}

/*
Convenient Filename Hacks
*/

FILE *cfh_fopen(const char *filename, const char *mode)
{
	char filename1[MAX_PATH];
	
	fix_filename(filename, filename1);
	
	return fopen(filename1, mode);	
}

int Files::OpenFile(Str255 Name)
{
	char filename1[MAX_PATH];
	
	fix_filename((char *)Name, filename1);
	
	sFile = open(filename1, O_RDONLY | O_BINARY);
	return sFile;
}

void Files::EndLoad()
{
	if (sFile != -1) {
		FSClose( sFile );
	}
	
	sFile = -1;
}

    
void alutLoadWAVFile(char *filename, ALenum *format, void **wave,
	unsigned int *size, ALsizei *freq)
{
	char filename1[256];
	ALsizei format1, size1, bits1, freq1;
	int i, len;
	
	fix_filename(filename, filename1);
	
	alutLoadWAV(filename1, wave, &format1, &size1, &bits1, &freq1);
	
	*format = format1;
	*size = size1;
	*freq = freq1;
}

void alutUnloadWAV(ALenum format, void *wave, unsigned int size,
	ALsizei freq)
{
	free(wave);
}
