#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#ifdef USE_MTRACE
#include <mcheck.h>
#endif
#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define BLOCK_SIZE 0x10000
#define N_ITERS 10000

int invalid_operations(int level, int is_array)
{
	int i, *p;
	clock_t t;
	t = clock(); srand(1);
	for (i = 0; i != N_ITERS; ++i) {
		p = (int*)malloc(sizeof(int) * (rand()&0xfffff));
		p[0] = 0;
		free(p);
	}
	p = 0;
	fprintf(stderr, "Time elapses: %.3f sec.\n", (float)(clock() - t) / CLOCKS_PER_SEC);
	{
		int *q, r[BLOCK_SIZE];
		p = (int*)malloc(sizeof(int) * BLOCK_SIZE);
		q = (int*)malloc(sizeof(int) * BLOCK_SIZE); /* use q to "protect" the end of p */
		fprintf(stderr, "address - end of p: %lx, start of q: %lx\n", (unsigned long)p+sizeof(int)*BLOCK_SIZE,
				(unsigned long)q);
		if (level >= 1 && is_array) {
			fprintf(stderr, "(1) Test for invalid read/write on an array...");
			if (r[BLOCK_SIZE] == 6) p[0] = 1;
			r[BLOCK_SIZE+1] = 2;
			fprintf(stderr, "done.\n");
		}
		if (level >= 2) {
			fprintf(stderr, "(2) Test for comparing uninitialized variables...");
			if (p[0] == 5) p[0] = 1;
			fprintf(stderr, "done\n");
		}
		if (level >= 3) {
			fprintf(stderr, "(3) Test for invalid read...");
			if (p[BLOCK_SIZE+9] == 6) p[0] = 2;
			if (p[BLOCK_SIZE+100] == 6) p[0] = 3;
			fprintf(stderr, "done.\n");
		}
		if (level >= 4) {
			fprintf(stderr, "(4) Test for invalid write...");
			p[BLOCK_SIZE] = 0;
			p[BLOCK_SIZE+100] = 100;
			fprintf(stderr, "done\n");
		}
		free(p);
	}
	if (level >= 5) {
		fprintf(stderr, "(5) Test for double free...");
		free(p);
		fprintf(stderr, "done\n");
	}
	return 0;
}
int main(int argc, char *argv[])
{
	int c, is_array, level;
	is_array = 1;
	level = 10;
	while ((c = getopt(argc, argv, "l:ah")) >= 0) {
		switch (c) {
		case 'l': level = atoi(optarg); break;
		case 'a': is_array = 0; break;
		case 'h': fprintf(stderr, "Usage: %s [-l <level>] [-a]\n", argv[0]); return 1;
		}
	}
#ifdef USE_MTRACE
	setenv("MALLOC_TRACE", "mtrace.out", 1);
	mtrace();
#endif
	invalid_operations(level, is_array);
	return 0;
}
