#ifndef SUPPORT_H
#define SUPPORT_H

typedef unsigned char * Str255;
typedef int OSErr;

typedef struct UnsignedWide
{
	unsigned int lo;
	unsigned int hi;
} UnsignedWide;

void Microseconds (UnsignedWide *microTickCount);
OSErr FSRead(int refNum, long *count, void *buffPtr);

#endif
