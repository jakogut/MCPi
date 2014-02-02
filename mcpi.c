#include <stdio.h>
#include <math.h>
#include <time.h>

#include "mcpi_const.h"

vec_t calculate_pi()
{
	const vec_t increment = 1.0 / (vec_t)BUFFER_SIZE_SQRT;

	// GCC's auto vectorizer wants a constant value in the condition statement
	const unsigned it = BUFFER_SIZE_SQRT - 1;

	int_t hits = 0;
	for(int_t x = 0; x < it; ++x)
		for(int_t y = 0; y < it; ++y)
			hits += (powf(x * increment, 2) + powf(y * increment, 2) < 1);

	return (hits * 4) / (vec_t)(BUFFER_SIZE_SQRT * BUFFER_SIZE_SQRT);
}

int main()
{
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);

	printf("%f\n", calculate_pi());

	clock_gettime(CLOCK_MONOTONIC, &end);
	unsigned nsec_elapsed = (end.tv_nsec - start.tv_nsec) + (1000000000 * (end.tv_sec - start.tv_sec));
	double sec_elapsed = nsec_elapsed / 1000000000.0f;

	printf("%i Cycles, %i FLOP/cycle, %f sec elapsed\n%f GFLOPS\n\n",
		(int)pow(BUFFER_SIZE_SQRT, 2),
		FLOPS_PER_CYCLE,
		sec_elapsed,
		((pow(BUFFER_SIZE_SQRT, 2) * FLOPS_PER_CYCLE) / sec_elapsed) / 1000000000.0f);
}
