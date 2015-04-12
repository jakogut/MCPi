#include <stdio.h>
#include <math.h>
#include <time.h>

#include <CL/cl.h>

#include "mcpi_const.h"

#define SQUARE(n) ((n) * (n))

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

FILE *mcpi_fopen(const char *filename, const char *extension, const char *mode)
{
	FILE *pFile = NULL;
	size_t fname_chars = strlen(filename) + strlen(extension) + 1;
	char *fname = calloc(fname_chars, sizeof(char));
	int i;

	strcpy(fname, filename);

	for(i = 0; i < 2 && !pFile; i++)
	{
		if(i > 0) strcat(fname, extension);
		pFile = fopen(fname, mode);
	}

	free(fname);
	return pFile;
}

int mcpi_fcopy(char *dest, size_t size, FILE* src)
{
	size_t i;
	long pos = ftell(src);

	for(i = 0; i < size && !feof(src); i++) dest[i] = fgetc(src);
	dest[i - 1] = 0;

	fseek(src, pos, SEEK_SET);

	return 0;
}

int mcpi_flength(FILE *f)
{
	int length;
	long pos = ftell(f);

	for(length = 0; !feof(f); length++) fgetc(f);
	fseek(f, pos, SEEK_SET);

	return length;
}

int main()
{
	cl_int error = 0;

	cl_platform_id platform;
	oclGetPlatformID(&platform);

	cl_device_id device;
	cl_context context = clCreateContext(0, 1, &device, NULL, NULL, &error);

	cl_command_queue queue = clCreateCommandQueue(context, device, 0, &error);

	short* out_h = calloc(SQUARE(BUFFER_SIZE_SQRT), sizeof(short));
	cl_mem out_d = clCreateBuffer(context, CL_MEM_WRITE_ONLY, SQUARE(BUFFER_SIZE_SQRT) * sizeof(short), NULL, &error);

	// -- Compile program -- //
	FILE* f = mcpi_fopen("mcpi", ".cl", "r");
	unsigned src_size = mcpi_flength(f);

	char* source = calloc(1, src_size);
	mcpi_fcopy(source, src_size, )
	cl_program clCreateProgramWithSource(context, 1, , NULL, &error);

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
