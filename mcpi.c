#include <stdio.h>
#include <math.h>

#include "mcpi_const.h"

vec_t calculate_pi()
{
	vec_t increment = 1.0 / (vec_t)BUFFER_SIZE_SQRT;

	int hits = 0;
	for(int x = 0; x < BUFFER_SIZE_SQRT; x++)
		for(int y = 0; y < BUFFER_SIZE_SQRT; y++)
			if((powf(x * increment, 2) + powf(y * increment, 2)) < 1)
				++hits;

	return (hits * 4) / (vec_t)(BUFFER_SIZE_SQRT * BUFFER_SIZE_SQRT);
}

int main()
{
	printf("%f\n", calculate_pi());
}
