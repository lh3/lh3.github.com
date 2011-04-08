/*
  mem.c -- light-weighted memory allocator

  Copyright (c) 2007, Heng Li <lh3lh3@gmail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include "mem.h" */

/* The whole thing is: ("@" for the KR_HEADER of the block, "-" for free memory, and "+" for allocated memory.
   One char for one unit.)
 *                        
 *           This region is core 1.                             This region is core 2.
 *
 *   @-------@++++++@++++++++++++@------------           @----------@++++++++++++@+++++++@------------
 *   |                           |                       |                               |
 *   p=p->ptr->ptr->ptr->ptr     p->ptr             p->ptr->ptr                p->ptr->ptr->ptr
 */

#ifndef kr_size(x)
#define kr_size(x) ((((KR_HEADER*)(x))-1)->size * sizeof(KR_HEADER))
#endif

typedef struct _KR_HEADER_
{
	struct _KR_HEADER_ *ptr; /* next free block */
	size_t size; /* size of current free block */
} KR_HEADER;

static KR_HEADER base;
static KR_HEADER *loop_head = 0; /* the last and also the first free block */
static size_t kr_total_allocated = 0, kr_n_core = 0;

void kr_error(const char *s)
{
	fprintf(stderr, "FATAL MEMORY PROBLEM: %s\n", s);
	exit(1);
}
static KR_HEADER *morecore(size_t nu)
{
	extern float kr_stat();
	extern void kr_free(void*);
	size_t rnu;
	KR_HEADER *up;

	rnu = (nu + 0xffff) & (~(size_t)(0xffff));
	up = (KR_HEADER*)malloc(rnu * sizeof(KR_HEADER));
	if (!up) { /* fail to allocate memory */
		kr_stat();
		fprintf(stderr, "*morecore* %u bytes requested but not available.\n", rnu * sizeof(KR_HEADER));
		exit(1);
	}
	kr_total_allocated += rnu * sizeof(KR_HEADER);
	++kr_n_core;
	up->size = rnu; /* the size of the current block, and in this case the block is the same as the new core */
	kr_free(up + 1); /* initialize the new "core" */
	return loop_head;
}
void kr_free(void *ap)
{
	KR_HEADER *p, *q;
	
	if (!ap) return;
	p = (KR_HEADER*)ap - 1; /* p->size is the size of the current block */
	/* Find the pointer that points to the block to be freed. The following loop can stop on two conditions:
	 *
	 * a) "p>q && p<q->ptr": @------@++++++++@+++++++@-------    @---------------@+++++++@-------
	 *    (can also be in    |      |                |        -> |                       |
	 *     two cores)        q      p           q->ptr           q                  q->ptr
	 *
	 *                       @--------    @+++++++++@--------    @--------    @------------------
	 *                       |            |         |         -> |            |
	 *                       q            p    q->ptr            q       q->ptr
	 *
	 * b) "q>=q->ptr && (p>q || p<q->ptr)":  @-------@+++++   @--------@+++++++     @-------@+++++   @----------------
	 *                                       |                |        |         -> |                |
	 *                                  q->ptr                q        p       q->ptr                q
	 *
	 *                                       @+++++++@-----   @++++++++@-------     @-------------   @++++++++@-------
	 *                                       |       |                 |         -> |                         |
	 *                                       p  q->ptr                 q       q->ptr                         q
	 */
	for (q = loop_head;; q = q->ptr) {
		if (q < q->ptr) {
			if (q < p && p < q->ptr) break;
		} else {
			if (p > q || p < q->ptr) break;
		}
	}
	if (p + p->size == q->ptr) { /* two adjacent blocks, merge p and q->ptr (the 2nd and 4th cases) */
		p->size += q->ptr->size; /* this is the new q->ptr size */
		p->ptr = q->ptr->ptr; /* this is the new q->ptr->ptr */
		/* p is actually the new q->ptr. The actual change happens a few lines below. */
	} else if (p + p->size > q->ptr && q->ptr >= p) { /* the end of the allocated block is in the next free block */
		kr_error("*kr_free* The end of the allocated block enters a free block.");
	} else p->ptr = q->ptr; /* backup q->ptr */

	if (q + q->size == p) { /* two adjacent blocks, merge q and p (the other two cases) */
		q->size += p->size;
		q->ptr = p->ptr;
	} else if (q + q->size > p && p >= q) { /* the end of a free block in the allocated block */
		kr_error("*kr_free* The end of a free block enters the allocated block.");
	} else q->ptr = p; /* in two cores, cannot be merged */
	loop_head = q;
}
void *kr_realloc(void *ap, size_t n_bytes)
{
	extern void *kr_malloc(size_t);
	KR_HEADER *p, *q;
	size_t n_units;
	
	if (!ap) return kr_malloc(n_bytes);
	n_units = 1 + (n_bytes + sizeof(KR_HEADER) - 1) / sizeof(KR_HEADER);
	p = (KR_HEADER*)ap - 1;
	if (p->size >= n_units) return ap;
	q = (KR_HEADER*)kr_malloc(n_bytes);
	memcpy(q, ap, (p->size  - 1) * sizeof(KR_HEADER));
	kr_free(ap);
	return q;
}
void *kr_malloc(size_t n_bytes)
{
	KR_HEADER *p, *q;
	size_t j, n_units;
	int i;

	/* "n_units" means the number of units. The size of one unit equals to sizeof(KR_HEADER).
	 * "1" is the KR_HEADER of a block, which is always required. */
	n_units = 1 + (n_bytes + sizeof(KR_HEADER) - 1) / sizeof(KR_HEADER);
#ifndef KR_NO_POW2
	for (i = 31, j = (size_t)0x80000000L; i >= 0; --i, j >>= 1)
		if (n_units & j) { n_units = (size_t)1<<(i+1); break; }
#endif
	if (!(q = loop_head)) { /* the first time when kr_malloc() is called, intialization */
		base.ptr = loop_head = q = &base;
		base.size = 0;
	}
	for (p = q->ptr;; q = p, p = p->ptr) { /* search for a suitable block */
		if (p->size >= n_units) { /* p->size if the size of current block. This line means the current block is large enough. */
			if (p->size == n_units) q->ptr = p->ptr; /* no need to split the block */
			else { /* split the block */
				/* memory is allocated at the end of the block */
				p->size -= n_units; /* reduce the size of the free block */
				p += p->size; /* skip to the KR_HEADER of the allocated block */
				p->size = n_units; /* set the size */
			}
			loop_head = q; /* set the end of chain */
			return p + 1; /* skip the KR_HEADER */
		}
		if (p == loop_head) /* then ask for more "cores" */
			if (!(p = morecore(n_units))) return 0; /* p==0 if fail to allocate, but morecore() will call exit(1) first */
	}
}
float kr_stat()
{
	unsigned n_blocks, n_units;
	size_t max_block = 0;
	KR_HEADER *p, *q;
	float frag;

	if (loop_head == 0) {
		fprintf(stderr, "<kr_stat> No memory has been allocated so far.");
		return 0.0;
	}
	p = loop_head;
	n_blocks = n_units = 0;
	do {
		q = p->ptr;
		if (p->size > max_block) max_block = p->size;
		n_units += p->size;
		if (p + p->size > q && q > p)
			kr_error("*kr_stat* The end of a free block enters another free block.");
		p = q;
		++n_blocks;
	} while (p != loop_head);
	
	--n_blocks;
	frag = 1.0/1024.0*n_units * sizeof(KR_HEADER)/kr_n_core;
	fprintf(stderr, "<kr_stat> Total=%u, Free=%u, C=%u, B=%u, Max=%u, Frag=%.3fK\n",
			kr_total_allocated, n_units * sizeof(KR_HEADER), kr_n_core, n_blocks, max_block, frag);
	return frag;
}

#ifdef KR_TEST
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
	char **p;
	int i, j, k, n, m, N, do_alloc;
	n = 20000; N = 40000; m = 1024;
	srand48(time(0));
	p = (char**)kr_malloc(sizeof(char*) * N);
	kr_stat();
	for (i = 0; i < N; ++i) p[i] = 0;
	for (i = j = 0; i < N; ++i) {
		do_alloc = (drand48() < 1.0 - (double)j/n)? 1 : 0;
		if (j == 0) do_alloc = 1;
		else if (j == n) do_alloc = 0;
		if (do_alloc == 1) {
			if (drand48() > 0.5) {
				p[j++] = (char*)kr_malloc(sizeof(char) * (int)(m * drand48() + 1.5));
			} else {
				k = (int)(drand48() * j);
				p[k] = (char*)kr_realloc(p[k], sizeof(char) * (int)(m * (1.0 + drand48()) + 0.5));
				if (k == j) ++j;
			}
		} else if (do_alloc == 0) {
			k = (int)(drand48() * j);
			kr_free(p[k]); p[k] = 0;
		}
		if (i != 0 && i % 1000 == 0) kr_stat();
	}
	fprintf(stderr, "%d\n", kr_size(p));
	for (i = 0; i < N; ++i)
		kr_free(p[i]);
	kr_free(p);
	kr_stat();
	return 0;
}
#endif /* KR_TEST */
