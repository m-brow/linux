/*
 * VSX memset functions
 *
 * Copyright 2017 IBM Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <altivec.h>
#include <linux/string.h>

#define __HAVE_ARCH_MEMSET16
void *memset16(uint16_t *s, uint16_t v, size_t count)
{
	const vector unsigned short x = {v,v,v,v, v,v,v,v};
	uint16_t *xs = s;
	int i = 0;

	while (i < count) {
		if (i > 7) {
			vec_vsx_st(x, 0, xs);
			xs += 8;
			i  += 8;
		}
		else {
			*xs++ = v;
			i++;
		}
	}
	return s;
}

#define __HAVE_ARCH_MEMSET32
void *memset32(uint32_t *s, uint32_t v, size_t count)
{
	const vector unsigned int x = {v, v, v, v};
	uint32_t *xs = s;
	int i = 0;

	while (i < count) {
		if (i > 15) {
			vec_vsx_st(x, 0, xs);
			vec_vsx_st(x, 0, (xs + 4));
			vec_vsx_st(x, 0, (xs + 8));
			vec_vsx_st(x, 0, (xs + 12));
			xs += 16;
			i += 16;
		}
		else if (i > 7) {
			vec_vsx_st(x, 0, xs);
			vec_vsx_st(x, 0, (xs + 4));
			xs += 8;
			i += 8;
		}
		else if (i > 3) {
			vec_vsx_st(x, 0, xs);
			xs += 4;
			i  += 4;
		}
		else {
			*xs++ = v;
			i++;
		}
	}
	return s;
}
#ifdef CONFIG_PPC64
#define __HAVE_ARCH_MEMSET64
void *memset64(uint64_t *s, uint64_t v, size_t count)
{
	const vector unsigned long x = {v, v};
	uint64_t *xs = s;
	int i = 0;

	while (i < count) {
		if (i > 7) {
			*(vector unsigned long *)xs = x;
			*(vector unsigned long *)(xs + 2) = x;
			*(vector unsigned long *)(xs + 4) = x;
			*(vector unsigned long *)(xs + 6) = x;
			xs += 8;
			i  += 8;

		}
		else if (i > 3) {
			*(vector unsigned long *)xs = x;
			*(vector unsigned long *)(xs + 2) = x;
			xs += 4;
			i += 4;
		}
		else if (i > 1) {
			*(vector unsigned long *)xs = x;
			xs += 2;
			i  += 2;
		}
		else {
			*xs++ = v;
			i++;
		}
	}
	return s;
}
#endif	/* CONFIG_PPC64 */
