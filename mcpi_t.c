#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <pthread.h>

#include "mcpi_const.h"

#define MIN_THREADS 1
#define MAX_THREADS 16

// How much of a performance gain is enough to consider statistically significant
#define GAIN_FACTOR 0.05

typedef struct { int thread_id; int* num_threads; int_t* hits; } thread_args;

void* calculate_pi_thread(void* arg)
{
	thread_args* args = (thread_args*)arg;

	const unsigned work_size = BUFFER_SIZE_SQRT / *args->num_threads;
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
	double* runtime_sec_elapsed = calloc(sizeof(double), (MAX_THREADS - MIN_THREADS) + 1);

	for(int run = 0; run <= MAX_THREADS - MIN_THREADS; run++)
	{
		int num_threads = run + MIN_THREADS;

		thread_args targs[num_threads];
		int_t hits[num_threads];

		for(int i = 0; i < num_threads; i++)
		{
			targs[i].thread_id = i;
			targs[i].num_threads = &num_threads;
			targs[i].hits = (int_t*)hits;
		}

		pthread_t calcpi[num_threads];

		struct timespec start, end;
	        clock_gettime(CLOCK_MONOTONIC, &start);

		for(int i = 0; i < num_threads; i++) pthread_create(&calcpi[i], NULL, calculate_pi_thread, (void*)&targs[i]);
		for(int i = 0; i < num_threads; i++) pthread_join(calcpi[i], NULL);

	        clock_gettime(CLOCK_MONOTONIC, &end);
	        unsigned nsec_elapsed = (end.tv_nsec - start.tv_nsec) + (1000000000 * (end.tv_sec - start.tv_sec));
	        runtime_sec_elapsed[run] = nsec_elapsed / 1000000000.0f;

		int_t hits_sum = 0;
		for(int i = 0; i < num_threads; i++)
			hits_sum += hits[i];

		// Uncomment to print PI approximations for each run
		//printf("%f\n", (hits_sum * 4) / (vec_t)(BUFFER_SIZE_SQRT * BUFFER_SIZE_SQRT));
	}

	int best_run = 0;
	for(int i = 0; i <= MAX_THREADS - MIN_THREADS; i++)
	{
		double* best = &runtime_sec_elapsed[best_run];
		if(runtime_sec_elapsed[i] < *best - (*best * GAIN_FACTOR)) best_run = i;
	}

	printf("%i threads found to be optimal\n%i Cycles, %i FLOP/cycle, %f sec elapsed\n%f GFLOPS\n\n",
		(best_run + MIN_THREADS),
		(int)pow(BUFFER_SIZE_SQRT, 2),
		FLOPS_PER_CYCLE,
		runtime_sec_elapsed[best_run],
		((pow(BUFFER_SIZE_SQRT, 2) * FLOPS_PER_CYCLE) / runtime_sec_elapsed[best_run]) / 1000000000.0f);

	free(runtime_sec_elapsed);
}
