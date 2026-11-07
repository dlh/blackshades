#ifndef SUPPORT_H
#define SUPPORT_H

#include <stdio.h>
#include <unistd.h>

#include <AL/al.h>

struct SDL_Window;
extern SDL_Window *gWindow;

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#define STUB_FUNCTION fprintf(stderr,"STUB: %s at " __FILE__ ", line %d, thread %d\n",__FUNCTION__,__LINE__,getpid())

#define fsFromStart SEEK_SET

typedef unsigned char * Str255;
typedef int OSErr;
typedef short int SInt16;

typedef bool Boolean;
#define TRUE true
#define FALSE false

typedef struct Point
{
	int h;
	int v;
} Point;

#define SetFPos(fildes, whence, offset) lseek(fildes, offset, whence)
#define FSClose(fildes) close(fildes)

int Random();
void GetMouse(Point *p);
void GetMouseRel(Point *p);
void GetKeys(unsigned char *keys);
int Button(void);
void LoadOGG_CFH(char *filename, ALenum *format, void **wave, 
	unsigned int *size, ALsizei *freq);
void FreeOGG(ALenum format, void *wave, unsigned int size, 
	ALsizei freq);


void fix_filename(const char *original, char *fixed);
void LocateGameData();

extern char gConfigPath[MAX_PATH];
extern char gHighscorePath[MAX_PATH];
void LocateSaveFiles();

#endif
