#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "rtclock.h"

typedef struct thread_args {
	int tid;
	int begin;
	int end;
} thread_args;

#define SQNT "S"
#define PRLL "P"

extern int* matA;
extern int* matB;
extern int* matC;
extern int* matD;
extern int size;

void intlMat(int* mat);
void seqmmm(int* mata, int* matb, int* result);
void parmmm(void* args);
void printMatrix(int* mat);