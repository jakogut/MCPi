#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <pthread.h>

#define NUM_THREADS 2

#define BUFFER_SIZE 64000000
float sample_buffer[BUFFER_SIZE][2];

void randomize_buffer()
{
        for(int i = 0; i < BUFFER_SIZE; i++)
                for(int j = 0; j < 2; j++)
                        sample_buffer[i][j] = rand() / (float)RAND_MAX;
}

typedef struct { int thread_id; int* hits; } calculate_pi_args;

void* calculate_pi_thread(void* arg)
{
	calculate_pi_args* args = (calculate_pi_args*)arg;
	args->hits[args->thread_id] = 0;

	int work_size = BUFFER_SIZE / NUM_THREADS;
	int work_start = work_size * args->thread_id;
	int work_end = work_start + work_size;

	for(int i = work_start; i < work_end; i++)
		if((powf(sample_buffer[i][0], 2) + powf(sample_buffer[i][1], 2)) < 1)
			args->hits[args->thread_id]++;
}

int main()
{
	srand(time(NULL));
	randomize_buffer();

	calculate_pi_args calc_args[NUM_THREADS];
	int hits[NUM_THREADS];

	for(int i = 0; i < NUM_THREADS; i++)
	{
		calc_args[i].thread_id = i;
		calc_args[i].hits = (int*)hits;
	}

	pthread_t calcpi[NUM_THREADS];

	for(int i = 0; i < NUM_THREADS; i++)
		pthread_create( &calcpi[i], NULL, calculate_pi_thread, (void*)&calc_args[i]);

	for(int i = 0; i < NUM_THREADS; i++)
		pthread_join(calcpi[i], NULL);

	int hits_sum = 0;
	for(int i = 0; i < NUM_THREADS; i++)
		hits_sum += hits[i];

	printf("%f\n", (hits_sum * 4) / (float)BUFFER_SIZE);
}
