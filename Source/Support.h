#ifndef SUPPORT_H
#define SUPPORT_H

#include <stdio.h>
#include <unistd.h>

#define STUB_FUNCTION fprintf(stderr,"STUB: %s at " __FILE__ ", line %d, thread %d\n",__FUNCTION__,__LINE__,getpid())

#define fsFromStart SEEK_SET

typedef unsigned char * Str255;
typedef int OSErr;

typedef bool Boolean;
#define TRUE true
#define FALSE false

typedef struct UnsignedWide
{
	unsigned int lo;
	unsigned int hi;
} UnsignedWide;

typedef struct Point
{
	int h;
	int v;
} Point;

#define Random rand
#define SetFPos(fildes, whence, offset) lseek(fildes, offset, whence)
#define FSClose(fildes) close(fildes)
#define FSRead(fildes, count, buf) read(fildes, buf, *count)

void Microseconds(UnsignedWide *microTickCount);
void GetMouse(Point *p);
void GetKeys(unsigned long *keys);
int Button(void);


#endif
