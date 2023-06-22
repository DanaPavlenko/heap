/* 
 * @file   heap.c
 * @author Dana
 *
 */
 
#include "heap.h"

static bool isheap(const void* src)
{
	if (src >= first && src <= (char*)first + SIZE) {
		return true;
	} 
	else { 
		return false;
	}
}

static bool isfree(size_t size)
{
	size_t i = (size_t)ptr - (size_t)first;
	while (size--) {
		if (mapget(i++) != 0) {
			return false;
		}
	}
	return true;
}

static bool find (size_t size)
{
	size_t count = 0;
	for (int i = 0; i < SIZE; i++) {
		while (mapget(i++) == 0) {
			count++;
			if (count == size) {
				ptr = (char*)first + i - size;
				return true;
			}
		}
		count = 0;
	}
	return false;
}

void* mymalloc (size_t size)
{
	last = ptr;
	size_t size_res = size / 2;
	if (size == 0 || SIZE - busy < size + size_res) {
		return NULL;
	} 
	else {
		if (isfree(size)) {
			size_t i = (size_t)ptr - (size_t)first;
			busy += size + size_res;
			ptr = (void*)((int)ptr + size + size_res);

			while(--size) {
				mapset(i++, 1);
			}
			mapset(i++, 3);
			while (size_res--) {
				mapset(i++, 2);	
			}
			return last;
		} 
		else if (find(size)) {
			mymalloc(size);
		} 
		else {
			return NULL;
		}
	}
}

void* mycalloc (size_t count, size_t size)
{
	void* p = mymalloc(count * size);
	if (p) {
		mymemset(p, 0, count * size);
	}
	return p;
}

void* myrealloc (void* src, const size_t size)
{
	if (src == NULL) {
		return mymalloc(size);
	}
	else if (size == 0) {
		 myfree(src);
		 return NULL;
	} 
	else if (isheap(src)) {
		size_t count_busy = 1;
	    size_t i = (size_t)src - (size_t)first;
		while (mapget(i++) != 3) {
			count_busy++;
		}
		if (size < count_busy) {
			while (size != count_busy--) {
				mapset(--i, 2);
			}
			mapset(--i, 3);
			return src;
		}
		else {
			size_t j = i-1;
			size_t count_res = 0;
			while (mapget(i++) == 2) {
				count_res++;
			}
			if (size-count_busy <= count_res) {
				while (size != count_res++) {
					mapset(j++, 1);
				}
				mapset(j++, 3);
				return src;
			}
			else {
				void* p = mymalloc(size);
				if(p) {
					mymemcpy(p, src, count_busy);
					myfree(src);
				}
				return p;
			}
		}
	}
	else {
		return NULL;
	}
}

void myfree (void* src)
{
	if (isheap(src)) {
		size_t count = 0;
		size_t i = (size_t)src - (size_t)first;
		if (mapget(i) == 1 || mapget(i) == 3) {
			while (mapget(i)!=3) {
				mapset(i++, 0);
				count++;
			}
			while (mapget(i) != 1 && mapget(i) != 0) {
				mapset(i++, 0);
				count++;
			}
			busy-=count;
			if (src == last) {
				ptr = (void*)((int)ptr - count);
			}
		}
	}
}

void* mymemcpy (void* dst, const void* src, size_t size)
{
	if (isheap(dst) && isheap(src)) {
		if(dst != src){
			for (size_t i = 0; i < size; i++) {
				((char*)dst)[i] = ((char*)src)[i];	
			}
		}
		return dst;
	} 
	else {
		return NULL;
	}
}

void* mymemmove (void* dst, const void* src, size_t size)
{
	if (isheap(dst) && isheap(src)) {
		if(dst != src) {
			if (dst <= src || dst >=((char*)src + size)) {
				for (size_t i = 0; i < size; i++) {
					((char*)dst)[i] = ((char*)src)[i];
				}
			} 
			else {
				for (size_t i = size; i < 0; i--) {
					((char*)dst)[i] = ((char*)src)[i];
				}
			}
		}
		return dst;
	} 
	else {
		return NULL;
	}
}

void* mymemchr(const void* src, int val, size_t size) 
{
	if (isheap(src)) {
		for (size_t i = 0; i < size; i++) {
			if (((char*)src)[i] == val) {
				return (void*)((int)src + i);
			}
		}
	}
	return NULL;
}

void* mymemset(void* dst, int val, size_t size) 
{
	if (isheap(dst)) {
		for (size_t i = 0; i < size; i++) {
			((char*)dst)[i] = val;
		}
	}
	return dst;
}
