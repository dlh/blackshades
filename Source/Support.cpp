#include "Support.h"
#include "Files.h"

#include <sys/types.h>
#include <dirent.h>
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

static int find_filename(char *filename)
{
	char *ptr;
	char *cur;
	char *next;
	DIR *dir;
	struct dirent *dirent;
	
	if (access(filename, R_OK) == 0) {
		return 1;
	}
	
	ptr = filename;
	
	while (*ptr) {
		if (ptr == filename || *ptr == '/') {
			cur = ptr+1;
			if (*cur == 0) {
				/* hit the end */
				break;
			}
			
			next = strchr(cur, '/');
			
			if (ptr != filename) {
				*ptr = 0;
			}
			
			if (next) {
				*next = 0;
			}
			
			if (ptr == filename && *ptr == '/') {
				dir = opendir("/");
			} else {
				dir = opendir(filename);
			}
			
			if (dir == NULL) {
				if (ptr != filename) {
					*ptr = '/';
				}
				
				if (next) {
					*next = 0;
				}
				
				return 0;
			}
			
			while ((dirent = readdir(dir)) != NULL) {
				if (strcasecmp(cur, dirent->d_name) == 0
					&& (dirent->d_type == DT_UNKNOWN ||
					(dirent->d_type == DT_DIR && next != NULL))) {
					strcpy(cur, dirent->d_name);
					break;
				}
			}
			
			closedir(dir);
			
			if (ptr != filename) {
				*ptr = '/';
			}
			
			if (next) {
				*next = '/';
				ptr = next;
			} else {
				ptr++;
			}
		} else {
			ptr++;
		}
	}
	
	if (access(filename, R_OK) == 0) {
		return 1;
	}
	
	return 0;
}

static void fix_filename(const char *original, char *fixed)
{
	const char *start;
	int i;
	int len;

	start = original;
	if (original[0] == ':') {
		start = &original[1];
	}

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
	
	/* 
	   here we would try to see if the file is available (game dir),
	   else try another dir
	   
	   really, this function also needs a flag to indicate whether
	   it should only go to local (write) or both (read)
	 */

	if (find_filename(fixed) == 0) {
		fprintf(stderr, "find failed: %s\n", fixed);
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

    
/*
  Our own special magic version that fixes the filename.
 */
void alutLoadWAVFile_CFH(char *filename, ALenum *format, void **wave,
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

void alutUnloadWAV_CFH(ALenum format, void *wave, unsigned int size,
	ALsizei freq)
{
	free(wave);
}
