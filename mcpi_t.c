#include <stdio.h>
#include <math.h>

#include <pthread.h>

#include "mcpi_const.h"

typedef struct { int thread_id; int_t* hits; } thread_args;

void* calculate_pi_thread(void* arg)
{
	thread_args* args = (thread_args*)arg;

	const unsigned work_size = BUFFER_SIZE_SQRT / NUM_THREADS;
	const unsigned work_start = work_size * args->thread_id;

	const unsigned it_x = work_size - 1;
	const unsigned it_y = BUFFER_SIZE_SQRT - 1;

	const vec_t increment = 1.0 / (vec_t)BUFFER_SIZE_SQRT;

	register int_t hits = 0;
	for(unsigned x = 0; x < it_x; ++x)
		for(unsigned y = 0; y < it_y; ++y)
			hits += (powf((x + work_start) * increment, 2) + powf(y * increment, 2) < 1);

	// Huge performance hit if we increment this directly
	args->hits[args->thread_id] = hits;
	return NULL;
}

int main()
{
	thread_args targs[NUM_THREADS];
	int_t hits[NUM_THREADS];

	for(int i = 0; i < NUM_THREADS; i++)
	{
		targs[i].thread_id = i;
		targs[i].hits = (int_t*)hits;
	}

	pthread_t calcpi[NUM_THREADS];

	struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

	for(int i = 0; i < NUM_THREADS; i++)
		pthread_create(&calcpi[i], NULL, calculate_pi_thread, (void*)&targs[i]);

	for(int i = 0; i < NUM_THREADS; i++)
		pthread_join(calcpi[i], NULL);

        clock_gettime(CLOCK_MONOTONIC, &end);
        unsigned nsec_elapsed = end.tv_nsec - start.tv_nsec;
        double sec_elapsed = nsec_elapsed / 1000000000.0f;

	int_t hits_sum = 0;
	for(int i = 0; i < NUM_THREADS; i++)
		hits_sum += hits[i];

	printf("%f\n", (hits_sum * 4) / (vec_t)(BUFFER_SIZE_SQRT * BUFFER_SIZE_SQRT));

        printf("%i Cycles, %i FLOP/cycle, %f sec elapsed\n%f GFLOPS\n\n",
                (int)pow(BUFFER_SIZE_SQRT, 2),
                FLOPS_PER_CYCLE,
                sec_elapsed,
                ((pow(BUFFER_SIZE_SQRT, 2) * FLOPS_PER_CYCLE) / sec_elapsed) / 1000000000.0f);
}
