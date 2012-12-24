#include <stdio.h>
#include <math.h>

#include <pthread.h>

#include "mcpi_const.h"

typedef struct { int thread_id; float*** buffer; int* hits; } thread_args;

void* calculate_pi_thread(void* arg)
{
	thread_args* args = (thread_args*)arg;
	args->hits[args->thread_id] = 0;

	int work_size = BUFFER_SIZE_SQRT / NUM_THREADS;
	int work_start = work_size * args->thread_id;
	int work_end = work_start + work_size;

	float increment = 1.0 / (float)BUFFER_SIZE_SQRT;

	for(int x = work_start; x < work_end; x++)
		for(int y = 0; y < BUFFER_SIZE_SQRT; y++)
			if((powf((x * increment), 2) + powf(y * increment, 2)) < 1)
				args->hits[args->thread_id]++;

	return NULL;
}

int main()
{
	thread_args targs[NUM_THREADS];
	int hits[NUM_THREADS];

	for(int i = 0; i < NUM_THREADS; i++)
	{
		targs[i].thread_id = i;
		targs[i].hits = (int*)hits;
	}

	pthread_t calcpi[NUM_THREADS];

	for(int i = 0; i < NUM_THREADS; i++)
		pthread_create(&calcpi[i], NULL, calculate_pi_thread, (void*)&targs[i]);

	for(int i = 0; i < NUM_THREADS; i++)
		pthread_join(calcpi[i], NULL);


	int hits_sum = 0;
	for(int i = 0; i < NUM_THREADS; i++)
		hits_sum += hits[i];

	printf("%f\n", (hits_sum * 4) / (float)(BUFFER_SIZE_SQRT * BUFFER_SIZE_SQRT));
}
