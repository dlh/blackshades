#ifndef _GLHEADERS_H_
#define _GLHEADERS_H_

/* The renderer is fixed-function OpenGL, which needs a legacy (2.1) context. */
#ifdef __APPLE__
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#endif
