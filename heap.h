/* 
 * @file   heap.h
 * @author Dana
 *
 */

#ifndef HEAP_H
#define	HEAP_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define SIZE 128 * 1024 * 1024 // 1 << 29

void* const first = malloc(SIZE);
void* ptr = first;
void* last = first;
size_t busy = 0;
char map [SIZE/4] = {0};

inline void mapset( size_t n, unsigned char x) {
	map[n / 4] = map[n / 4] & ~(3 << 6 - (n % 4) * 2);
	map[n / 4] = map[n / 4] | (x << 6 - (n % 4) * 2);
}

inline unsigned char mapget(size_t n) {
	return (map[n / 4] >> (6 - (n % 4) * 2) & 3); 
}

void* mymalloc(size_t size);
void* mycalloc(size_t count, size_t size);
void* myrealloc(void* src, const size_t size);
void  myfree(void* src);

void* mymemcpy(void* dst, const void* src, size_t size);
void* mymemmove(void* dst, const void* src, size_t size);

void* mymemchr(const void* src, int val, size_t size);
void* mymemset(void* dst, int val, size_t size);

#endif	/* HEAP_H */
