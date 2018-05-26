#pragma once
#include <stdio.h>   
#include <stdlib.h>
#include <math.h>
//#define STEP 0.45f
//#define PATH_LENGTH 11
#define PATH_LENGTH_COUNTING 100
#define PRT_THRESHOLD 0.5f
#define MIN_DIV 0.1f
#define MIGRATIONS 100000
//#define POP_SIZE 32
//#define DIMENSION 8192
extern int DIMENSION;
//CUDA
#define NUM_BLOCK_RECOMENDED 16 
#define TEST_TIME 20


//MPI
extern int PROCESS_ID;
extern int WORLD_SIZE;
extern int MY_DEVICE_ID;
extern int POP_SIZE ;
extern float STEP ;
extern float PATH_LENGTH ;
//#define GRID_X 4
//#define GRID_Y 4
extern int GRID_N;
extern int GRID_X;
//extern int GRID_Y;
extern int METHOD; 
extern float RANGE;
extern bool IS_ERROR_PARAMETERS;
extern bool IS_WRITE_MIGRATIONS;
//extern int NUM_GROUP_EACH_GRID;
// MLSOMA2
extern int GROUPS_PER_NODE;
extern int SOMA_TYPE;
extern bool OP0;
extern bool OP1;
extern float OP1_THRESHOLD;
extern bool OP2;
extern int OP2_N;
extern char* MIGRATION_NAME;

// 2018 04 11
extern int TEST_FUNCTION_ID;

extern void InitParameters(int argc, char *argv[]);
extern void ShowHelp();