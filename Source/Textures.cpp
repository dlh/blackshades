/*
(c) 2008 Victor "ErV" Eremin, Voronezh, Russia.
mailto: ErV2005@rambler.ru, erv@box.vsi.ru
for non-ncommercial use only
*/
#include "Textures.h"
#include "Support.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLuint loadTexture(const char* filename_, GLenum minFilter, GLenum magFilter, bool mipmaps){
	char filename[MAX_PATH];
	fix_filename(filename_, filename);

	SDL_Surface *loaded = SDL_LoadPNG(filename);
	if (!loaded){
		fprintf(stderr, "couldn't load file %s: %s\n", filename, SDL_GetError());
		return 0;
	}
	SDL_Surface *surface = SDL_ConvertSurface(loaded, SDL_PIXELFORMAT_RGBA32);
	SDL_DestroySurface(loaded);
	if (!surface){
		fprintf(stderr, "couldn't convert file %s: %s\n", filename, SDL_GetError());
		return 0;
	}

	GLuint tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, surface->pitch / 4);

	//well, our textures are upside down. Fixing it here.
	SDL_FlipSurface(surface, SDL_FLIP_VERTICAL);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	if (mipmaps)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, surface->w, surface->h, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	SDL_DestroySurface(surface);
	return tex;
}

