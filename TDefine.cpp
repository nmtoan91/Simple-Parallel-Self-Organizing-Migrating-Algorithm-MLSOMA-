#include "TDefine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int PROCESS_ID = 0; 
int WORLD_SIZE = 1; 
int MY_DEVICE_ID = 0; 
 int POP_SIZE = 16;  
 float STEP = 0.3;
 float PATH_LENGTH = 5;
 int GRID_X = 2;
 int GRID_N = 1; 
 int DIMENSION = 32; 
 float RANGE = 512;
 int METHOD = 0;
 bool IS_ERROR_PARAMETERS = false;
 bool IS_WRITE_MIGRATIONS = true;
 char* MIGRATION_NAME;
 int GROUPS_PER_NODE = 2;
 int SOMA_TYPE = 1;  //0 : AlltoOne  ; 1: AlltoAll
 bool OP0 = false;  //combine the individuals have same position (not good) 
 bool OP1 = false;  // AlltoAll: Only target the better individuals
 float OP1_THRESHOLD = 0.01; 
 bool OP2 = true; //AlltoAll fix number of leader for each individual
 int OP2_N = 2;
 int TEST_FUNCTION_ID = 0;



#include "TestFunctions.h"
 void InitParameters(int argc, char *argv[])
 {
	 MIGRATION_NAME = "migration.csv";
	 GRID_N = 1;
	 int i = 1;
	 while (i < argc)
	 {
		 if (strcmp(argv[i], "-m") == 0)
		 {
			 if (i == argc - 1) {IS_ERROR_PARAMETERS = true; break;}
			 METHOD = atoi(argv[++i]);
			  if(METHOD <0) { IS_ERROR_PARAMETERS = true; break; }
		  }
		  else if (strcmp(argv[i], "-pop") == 0)
		 {
			  if (i == argc - 1) { IS_ERROR_PARAMETERS = true; break; }
			 POP_SIZE = atoi(argv[++i]);
			 if (POP_SIZE <= 0) { IS_ERROR_PARAMETERS = true; break; }
		 }
		 else if (strcmp(argv[i], "-step") == 0)
		 {
			 if (i == argc - 1) { IS_ERROR_PARAMETERS = true; break; }
			 STEP = atof(argv[++i]);
			 if (STEP <= 0) { IS_ERROR_PARAMETERS = true; break; }
		 }
		 else if (strcmp(argv[i], "-plen") == 0)
		 {
			 if (i == argc - 1) { IS_ERROR_PARAMETERS = true; break; }
			 PATH_LENGTH = atof(argv[++i]);
			 if (PATH_LENGTH <= 0) { IS_ERROR_PARAMETERS = true; break; }
		 }
		 else if (strcmp(argv[i], "-range") == 0)
		 {
			 if (i == argc - 1) { IS_ERROR_PARAMETERS = true; break; }
			 RANGE = atof(argv[++i]);
			 if (RANGE <= 0) { IS_ERROR_PARAMETERS = true; break; }
		 }
		 else if (strcmp(argv[i], "-d") == 0)
		 {
			 if (i == argc - 1) { IS_ERROR_PARAMETERS = true; break; }
			 DIMENSION = atoi(argv[++i]);
			 if (DIMENSION <= 0) { IS_ERROR_PARAMETERS = true; break; }
		 }
		 else if (strcmp(argv[i], "-grid") == 0)
		 {
			 if (i == argc - 1) { IS_ERROR_PARAMETERS = true; break; }
			 GRID_X  = atoi(argv[++i]);
			 if (GRID_X <= 0) { IS_ERROR_PARAMETERS = true; break; }
		 }
		 else if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-?") == 0)
		 {
			 IS_ERROR_PARAMETERS = true; break;
		 }
		 else if (strcmp(argv[i], "-w") == 0 )
		 {
			 IS_WRITE_MIGRATIONS = true; 
		 }
		 else if (strcmp(argv[i], "-ng") == 0)
		 {
			 if (i == argc - 1) { IS_ERROR_PARAMETERS = true; break; }
			 GROUPS_PER_NODE = atoi(argv[++i]);
			 if (GROUPS_PER_NODE <= 0) { IS_ERROR_PARAMETERS = true; break; }
		 }
		 else if (strcmp(argv[i], "-type") == 0) 
		 {
			 if (i == argc - 1) {  IS_ERROR_PARAMETERS = true; break; }
			 SOMA_TYPE = atoi(argv[++i]);
			 if (SOMA_TYPE < 0) { printf("ERROR SOMA_TYPE = : %d \n", SOMA_TYPE); IS_ERROR_PARAMETERS = true; break; }
		 }
		 else if (strcmp(argv[i], "-op0") == 0)
		 {
			 OP0 = OP1 = OP2  = false;
			 OP0 = true; 
		 }
		 else if (strcmp(argv[i], "-op1") == 0)
		 {
			 OP0 = OP1 = OP2  = false;
			 OP1 = true; 
		 }
		 else if (strcmp(argv[i], "-op2") == 0)
		 {
			 OP0 = OP1 = OP2  = false;
			 OP2 = true; 
		 }
		 else if (strcmp(argv[i], "-op1thresh") == 0)
		 {
			 if (i == argc - 1) { IS_ERROR_PARAMETERS = true; break; }
			 OP1_THRESHOLD = atof(argv[++i]);
			 if (SOMA_TYPE <= 0) { IS_ERROR_PARAMETERS = true; break; }
		 }
		 else if (strcmp(argv[i], "-op2n") == 0)
		 {
			 if (i == argc - 1) { IS_ERROR_PARAMETERS = true; break; }
			 OP2_N = atoi(argv[++i]);
			 if (SOMA_TYPE <= 0) { IS_ERROR_PARAMETERS = true; break; }
		 }
		 else if (strcmp(argv[i], "-tf") == 0) 
		 {
			 if (i == argc - 1) { IS_ERROR_PARAMETERS = true; break; }
			 TEST_FUNCTION_ID = atoi(argv[++i]);
			 if (TEST_FUNCTION_ID < 0) { IS_ERROR_PARAMETERS = true; break; }
		 }
		 else if (strcmp(argv[i], "-out") == 0)
		 {
			 MIGRATION_NAME = argv[++i];
		 }
		 else
		 {
			 printf("Can't understand : %s \n", argv[i]);
			 IS_ERROR_PARAMETERS = true;
		 }

		 i++;
	 }

	
		

	 GRID_N = (int)powf(GRID_X, DIMENSION);
	 if (IS_ERROR_PARAMETERS) ShowHelp();
 }
 void ShowHelp()
 {
	 printf("MLSOMA version 1.0.2 :\n");
	 printf(" -m    : Method (0|1)\n [default = %d]",METHOD);
	 printf(" -pop  : Pop size [default = %d] \n",POP_SIZE);
	 printf(" -step : Migration step (float) [default = %d] \n",STEP);
	 printf(" -plen : Path length (float) [default = %f]\n",PATH_LENGTH);
	 printf(" -range: Range (bound) (float) [default = %f]\n",RANGE);
	 printf(" -d    : Dimensions [default = %d]\n",DIMENSION);
	 printf(" -grid : Grid count per dimension [default = %d]\n",GRID_X);
	 printf(" -w    : Set write the migrations [default = %b]\n",IS_WRITE_MIGRATIONS);
	 printf(" -ng   : Number SOMA groups for each node \n [default = %d]",GROUPS_PER_NODE);
	 printf(" -type : SOMA type(0:Allto1;1:AlltoAll) [default = %d]\n",SOMA_TYPE);
	 printf(" -op0  : Optimization method 0 [default = %b]\n",OP0);
	 printf(" -op1  : Optimization method 1 [default = %b]\n",OP1);
	 printf("	-op1thresh  : Config the threashold [default = %f]\n",OP1_THRESHOLD);
	 printf(" -op2  : Fix number of leaders for each individuals [default = %b]\n",OP2);
	 printf("	-op2n  : Config number of leader [default = %d]\n",OP2_N);
	 printf(" -tf  : testfunction [default = %d]\n", TEST_FUNCTION_ID);

	 printf(" -help : Help! \n");
	 printf("\n");
	 exit(0);
 }