#ifndef __UTASM_UTIL_BITFIELD_H__
#define __UTASM_UTIL_BITFIELD_H__

#include "vector.h"

typedef vec_uint32_t bitfield_t;

static inline void bitfield_set(bitfield_t bitfield, int val, int pos)
{
	int floor = pos >> 5;
	int ceil = (pos >> 5) + 1;

	printf("val = %u\n", val);
	printf("val & (1 << 32 - pos) - 1 = %u\n", (val & (1 << 32 - pos) - 1) << pos);
	printf("val & ((1 << (32 - pos)) - 1) = %u\n", (val & ((1 << (32 - pos)) - 1)) << pos);
	/*if (!(pos & 31))
	{
		if (ceil >= bitfield.size) return;

		// 10000000 00000000
		//		  ^1111

		printf("val & (1 << 32 - pos) - 1 = %u", val & (1 << 32 - pos) - 1);
		printf("val & ((1 << (32 - pos)) - 1) = %u", val & ((1 << (32 - pos)) - 1));

		//bitfield.data[floor] |= (val & (1 << 32 - pos) - 1) << pos;
		//bitfield.data[ceil]
	}*/
}

#endif//__UTASM_UTIL_BITFIELD_H__
