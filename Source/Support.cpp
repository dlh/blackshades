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

int Files::OpenFile(Str255 Name)
{
	sFile = open((char *)Name, O_RDONLY | O_BINARY);
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
	
	strncpy(filename1, filename, sizeof(filename1));
	filename1[sizeof(filename1)-1] = 0;
	
	len = strlen(filename1);
	for (i = 0; i < len; i++) {
		if (filename1[i] == ':') {
			filename1[i] = '/';
		}
	}
	
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
