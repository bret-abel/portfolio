#include "mmm.h"
//(int*) malloc(sizeof(int) * size * size);


/*
	Initializes a matrix with random ints from 0 to 99.
	Pointer arithmetic with 2D arrays is kinda ugly.
*/
void intlMat(int* mat)
{
	int i,j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			// *(mat + i*size + j) same as mat[i][j]
			*(mat + i*size + j) = (rand()%99);
		}
	}
}

/*
	Multiply two matrices sequentially and store
	the result in another matrix. 
	@Params: mata: multiplicand
			 matb: multiplier
			 result: result
*/
void seqmmm(int* mata, int* matb, int* result)
{
	int i, j, k;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			for (k = 0; k < size; k++)
			{
				// result[i][j] += mata[i][k] * matb[k][j]
				*(result + i*size + j) += (*(mata + i*size + k)) * (*(matb + k*size + j));
			}
		}
	}
}

/*
	The parallel version of the algorithm above.
	Accomodates multi-threading.
*/
void parmmm(void *args)
{
	thread_args rows = *((thread_args*) args);
	int i, j, k;

	for (i = rows.begin; i < rows.end; i++)
	{
		for (j = 0; j < size; j++)
		{
			for (k = 0; k < size; k++)
			{
				// result[i][j] += mata[i][k] * matb[k][j]
				*(matD + i*size + j) += (*(matA + i*size + k)) * (*(matB + k*size + j));
			}
		}
	}
}

/*
	Prints a matrix
*/
void printMatrix(int* mat)
{
	int i,j;
	for(i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			printf("%-6d", *(mat + i*size + j));
		}
		printf("\n");
	}
}
