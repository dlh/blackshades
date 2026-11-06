#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <SDL3/SDL.h>
#include <vorbis/vorbisfile.h>

#include "Support.h"
#include "Files.h"
#include "Gamepad.h"

int Random()
{
#if RAND_MAX >= 65535
	return (rand() % 65535) - 32767;
#else
#error please fix this for your platform
#endif
}

/* SDL reports the mouse in window points; the game works in pixels. */
void GetMouse(Point *p)
{
	float x;
	float y;
	float density = SDL_GetWindowPixelDensity(gWindow);

	if (GamepadCursor(&x, &y)) {
		int width;
		int height;

		SDL_GetWindowSizeInPixels(gWindow, &width, &height);

		p->h = (int)(x * width);
		p->v = (int)(y * height);
		return;
	}

	SDL_GetMouseState(&x, &y);
	
	p->h = (int)(x * density);
	p->v = (int)(y * density);
}

void GetMouseRel(Point *p)
{
	/* carry the fractions over so slow mouse movement isn't lost */
	static float remainderx = 0;
	static float remaindery = 0;
	float x;
	float y;
	
	SDL_GetRelativeMouseState(&x, &y);
	GamepadLook(&x, &y);

	remainderx += x;
	remaindery += y;
	
	p->h = (int)remainderx;
	p->v = (int)remaindery;
	
	remainderx -= p->h;
	remaindery -= p->v;
}
int Button(void)
{
	return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) || GamepadButton();
}

void InitMouse()
{
// STUB_FUNCTION;
}

void MoveMouse(int xcoord, int ycoord, Point *mouseloc)
{
	/* the mouse is in relative mode during play, so there's nothing to warp */
	GetMouse(mouseloc);
}

void DisposeMouse()
{
// STUB_FUNCTION;
}

/*
The game opens everything relative to the directory holding Data/. Look in the
current directory, next to the executable and one level above it (a build dir).
*/
void LocateGameData()
{
	const char *base = SDL_GetBasePath();
	char path[MAX_PATH];
	
	if (access("Data/Textures/Font.png", R_OK) == 0) {
		return;
	}
	
	if (base != NULL) {
		const char *suffixes[] = { "", "../" };
		
		for (int i = 0; i < 2; i++) {
			snprintf(path, sizeof(path), "%s%sData/Textures/Font.png", base, suffixes[i]);
			
			if (access(path, R_OK) == 0) {
				snprintf(path, sizeof(path), "%s%s", base, suffixes[i]);
				
				if (chdir(path) == 0) {
					return;
				}
			}
		}
	}
	
	fprintf(stderr, "ERROR: could not find the Data directory\n");
	exit(EXIT_FAILURE);
}

#ifndef O_BINARY
#define O_BINARY 0
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
			if (*ptr == '/') {
				cur = ptr+1;
			} else {
				cur = ptr;
			}
			
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
					*next = '/';
				}
				
				return 0;
			}
			
			while ((dirent = readdir(dir)) != NULL) {
				if (strcasecmp(cur, dirent->d_name) == 0) {
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

void fix_filename(const char *original, char *fixed)
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
Read the requested OGG file into memory, and extract the information required
by OpenAL
*/
void LoadOGG_CFH(char *filename, ALenum *format, void **wave,
	unsigned int *size, ALsizei *freq)
{
	char filename1[MAX_PATH];
	ALsizei format1, size1, freq1;
	void *wave1;
	OggVorbis_File vf;
	vorbis_info *vi;
	FILE *fp;
	int current_section;
	char *buf;
	int asize;
	int err;
	int eof;

	const int endian = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? 1 : 0;

	/* try to find the real file (and place it in filename1) */
	fix_filename(filename, filename1);

	/* open it for reading */
	fp = fopen(filename1, "rb");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: unable to open %s\n", filename1);
		exit(EXIT_FAILURE);
	}
	
	/* open it up */
	err = ov_open(fp, &vf, NULL, 0);	
	if (err < 0) {
		fprintf(stderr, "ERROR: vorbis error %d opening %s\n", -err, filename1);
		exit(EXIT_FAILURE);
	}

	/* get the ogg information */
	vi = ov_info(&vf, -1);
	if (vi == NULL) {
		fprintf(stderr, "ERROR: vorbis error opening %s (ov_info failed)\n", filename1);
		exit(EXIT_FAILURE);
	}
	
	/* calculate the byte size */
	size1 = vi->channels * 2 * ov_pcm_total(&vf, -1);

	/* hack around some possible ogg vorbis weirdness */
	asize = ((size1 + 2047) / 2048 + 1) * 2048;
	
	/* allocate our buffer */	
	wave1 = malloc(asize);
	
	if (wave1 == NULL) {
		fprintf(stderr, "ERROR: could not allocate %d bytes while loading %s\n", size1, filename1);
		exit(EXIT_FAILURE);
	}

	/* read it in */
	eof = 0;
	buf = (char *)wave1;
	
	while(!eof) {
		int space = asize - (int)(buf - (char *)wave1);
		long ret;
		
		if (space <= 0) {
			break;
		}
		
		ret = ov_read(&vf, buf, space < 1024 ? space : 1024, endian, 2, 1, 
			&current_section);
		
		if (ret == 0) {
			/* end of file */
			eof = 1;
		} else if (ret == OV_HOLE) {
			/* a gap in the data; the decoder picks up after it */
		} else if (ret < 0) {
			fprintf(stderr, "ERROR: vorbis error %ld reading %s\n", -ret, filename1);
			eof = 1;
		} else {
			buf += ret;
		}
	}
	
	size1 = (ALsizei)(buf - (char *)wave1);

	/* get the rest of the information */
	if (vi->channels == 1) {
		format1 = AL_FORMAT_MONO16;
	} else if (vi->channels == 2) {
		format1 = AL_FORMAT_STEREO16;
	} else {
		fprintf(stderr, "ERROR: ogg %s has %d channels\n", filename1, vi->channels);
		exit(EXIT_FAILURE);
	}
	
	freq1 = vi->rate;

	/* we are done with the ogg, so free it */
	ov_clear(&vf);
	
	/* finall, give the values to the caller */	
	*format = format1;
	*size = size1;
	*freq = freq1;
	*wave = wave1;
}

/*
Free the OGG buffer
*/
void FreeOGG(ALenum format, void *wave, unsigned int size,
	ALsizei freq)
{
	free(wave);
}
