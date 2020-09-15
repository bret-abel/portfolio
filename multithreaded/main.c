#include "mmm.h"
pthread_t *threads;
int threadCount;
int* matA;
int* matB;
int* matC;
int* matD;
int size;	// square matrix dimensions
double start, stop, seqtime, partime; //timer
int i; // need an increment counter in multiple places
int tester;

int main(int argc, char *argv[])
{
	if(argc < 3 || argc > 4)
	{
		printf("Error: Invalid number of arguments\n");
		exit(EXIT_FAILURE);
	}
	// sequential matrix multiplication
	else if (argc == 3 && !strcmp(argv[1],SQNT))
	{
		printf("---------------\n");
		printf("Mode: Sequential\n");
		printf("Number of Threads: 1\n");
		printf("Matrix Dimensions: %s x %s\n", argv[2], argv[2]);
		printf("---------------\n");
		
		// convert args string to int
		size = atoi(argv[2]);
		
		// allocate memory for matrices
		matA = (int*) malloc(sizeof(int) * size * size);
		matB = (int*) malloc(sizeof(int) * size * size);
		matC = (int*) malloc(sizeof(int) * size * size);

		// Check that memory allocation worked
		if (matA && matB && matC)
		{
			start = rtclock();
			
			// initialize matrices
			intlMat(matA);
			intlMat(matB);

			// multiply matrices
			// A x B = C
			seqmmm(matA, matB, matC);
			
			// printMatrix(matA);
			// printf("x\n");
			// printMatrix(matB);
			// printf("=\n");
			// printMatrix(matC);

			stop = rtclock();
			printf("Sequential Multiplication time: %f seconds\n", stop-start);

		}
	}
	// parallel matrix multiplication
	else if (argc == 4 && !strcmp(argv[1],PRLL))
	{

		printf("---------------\n");
		printf("Mode: Parallel\n");
		printf("Number of Threads: %s\n", argv[2]);
		printf("Matrix Dimensions: %s x %s\n", argv[3], argv[3]);
		printf("---------------\n");

		// convert arg strings into ints
		size = atoi(argv[3]);
		threadCount = atoi(argv[2]);

		// allocate memory
		matA = (int*) malloc(sizeof(int) * size * size);
		matB = (int*) malloc(sizeof(int) * size * size);
		matC = (int*) malloc(sizeof(int) * size * size);
		matD = (int*) malloc(sizeof(int) * size * size);
		threads = (pthread_t*) malloc(sizeof(pthread_t) * threadCount);

		// Initialize matrices (multiplicand and multiplier)
		intlMat(matA);
		intlMat(matB);
		
		// parallel multiplication
		start = rtclock();
		thread_args args[threadCount];
		for (i = 0; i < threadCount; i++)
		{
			// threads organized around number of rows in a matrix, each given a piece to work on
			args[i].tid = i;
			args[i].begin = i*size/threadCount;
			args[i].end = (i+1)*size/threadCount;
			pthread_create(&threads[i],NULL,parmmm,&args[i]);
		}
		// wait for threads to finish
		for (i = 0; i < threadCount; i++)
		{
			pthread_join(threads[i],NULL);
		}
		stop = rtclock();
		partime = stop - start;

		// Sequential Multiplication for comparison
		start = rtclock();
		seqmmm(matA,matB,matC);
		stop = rtclock();
		seqtime = stop - start;

		printf("Parallel Multiplication Time: %f\n",partime);
		printf("Sequential Multiplication Time: %f\n", seqtime);
		printf("Speedup: %f\n", seqtime/partime);
		int qualityControl = 0;
		for (i = 0; i < size; i++)
		{
			qualityControl += (*(matC + i) - *(matD + i));
		}
		printf("Sum of differences between values in sequential and parallel results: %d\n", qualityControl);

	}
	else
	{

		printf("Error: Invalid input\n");
		exit(EXIT_FAILURE);
	}

	free(matA);
	free(matB);
	free(matC);
	free(threads);
	exit(EXIT_SUCCESS);
}

