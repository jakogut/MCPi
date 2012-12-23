#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define BUFFER_SIZE_SQRT 4096

float*** create_buffer()
{
	float*** buffer = (float***)malloc(sizeof(float**) * BUFFER_SIZE_SQRT);
	for(int i = 0; i < BUFFER_SIZE_SQRT; i++)
	{
		buffer[i] = (float**)malloc(sizeof(float*) * BUFFER_SIZE_SQRT);

		for(int j = 0; j < BUFFER_SIZE_SQRT; j++)
			buffer[i][j] = (float*)malloc(sizeof(float) * 2);
	}

	return buffer;
}

void destroy_buffer(float*** buffer)
{
	for(int i = 0; i < BUFFER_SIZE_SQRT; i++)
	{
		for(int j = 0; j < BUFFER_SIZE_SQRT; j++)
			free(buffer[i][j]);

		free(buffer[i]);
	}

	free(buffer);
}

void fill_buffer(float*** buffer)
{
	float increment = 1.0 / (float)BUFFER_SIZE_SQRT;

	for(int x = 0; x < BUFFER_SIZE_SQRT; x++)
	{
		for(int y = 0; y < BUFFER_SIZE_SQRT; y++)
		{
			buffer[x][y][0] = x * increment;
			buffer[x][y][1] = y * increment;
		}
	}
}

float calculate_pi(float*** buffer)
{
	int hits = 0;
	for(int x = 0; x < BUFFER_SIZE_SQRT; x++)
		for(int y = 0; y < BUFFER_SIZE_SQRT; y++)
			if((powf(buffer[x][y][0], 2) + powf(buffer[x][y][1], 2)) < 1)
				++hits;

	return (hits * 4) / (float)(BUFFER_SIZE_SQRT * BUFFER_SIZE_SQRT);
}

int main()
{
	float*** sample_buffer = create_buffer();

	fill_buffer(sample_buffer);
	printf("%f\n", calculate_pi(sample_buffer));

	destroy_buffer(sample_buffer);
}
