#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define BUFFER_SIZE 64000000

float sample_buffer[BUFFER_SIZE][2];

void randomize_buffer()
{
	for(int i = 0; i < BUFFER_SIZE; i++)
		for(int j = 0; j < 2; j++)
			sample_buffer[i][j] = rand() / (float)RAND_MAX;
}

float calculate_pi()
{
	int hits = 0;
	for(int i = 0; i < BUFFER_SIZE; i++)
		if((powf(sample_buffer[i][0], 2) + powf(sample_buffer[i][1], 2)) < 1)
			++hits;

	return (hits * 4) / (float)((BUFFER_SIZE));
}

int main()
{
	srand(time(NULL));

	randomize_buffer();
	printf("%f\n", calculate_pi());
}
