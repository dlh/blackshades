#ifndef SUPPORT_H
#define SUPPORT_H

typedef struct UnsignedWide
{
	unsigned int lo;
	unsigned int hi;
} UnsignedWide;

void Microseconds (UnsignedWide *microTickCount);

#endif
