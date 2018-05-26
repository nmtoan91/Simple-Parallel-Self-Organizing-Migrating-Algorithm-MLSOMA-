#include "TimeAnalysis.h"
#include <stdio.h>
#include "TDefine.h"
#include <fstream>
#include <iostream>
#ifdef TMPI
#include"mpi.h"
#endif

#include "TestFunctions.h"
using namespace std;
TimeAnalysis::TimeAnalysis()
{
	Init();
	//TimeAnalysis::Instance = this;
}
TimeAnalysis::~TimeAnalysis()
{

} 
void TimeAnalysis::Init()
{
	TIME_TOTAL = 0; 
	NUM_TEST = NUM_RIGHT;
	TIME_PARSER = 0;
	TIME_TRANSFER_TO_GPPGU = 0;
	TIME_TRANSFER_TO_CPU = 0;
	TIME_GPU_KERNEL_CLOCK = 0;
	TIME_GPU_KERNEL_KERNEL = 0;
	TIME_CPU_HARDDISK = 0;
	TIME_LOAD_DATABASE = 0;
	TIME_CPU_DDRAM = 0;
	TIME_GPU_ALL = 0;
	SPEED_UP = 0;
	TIME_TRANSFER_DATA_TO_GPPGU = 0;
	DATA_SIZE = 0; 
	NUM_MIGRATION = 0;
	NUM_COST_FUNCTION_CALL = 0;

	POSITION = (float*)malloc(DIMENSION*sizeof(float));
} 
void TimeAnalysis::WriteResult(char* file_name,char* info )
{
	float accuracy = NUM_RIGHT*100.0f / NUM_TEST;
	accuracy = VALUE*100.0f / (418.982880f*DIMENSION);

	TIME_GPU_ALL = TIME_TRANSFER_TO_GPPGU + TIME_GPU_KERNEL_KERNEL + TIME_TRANSFER_TO_CPU;
	
	
	printf("\nPOP_SIZE = %d, ACCURACY = %f, COST_CALL=%d ",POP_SIZE, accuracy, NUM_COST_FUNCTION_CALL);
	SPEED_UP = TIME_CPU_DDRAM / TIME_GPU_ALL;
	FILE* f = NULL;   
	char name[256];
	if (file_name != NULL)
	{
		
		f = fopen(file_name, "wb+");
		if (f == NULL) printf("\n ERROR (TimeAnalysis): %s %n  \n", file_name,__LINE__ );
	}
	else
	{
		
		sprintf(name, "0000_%f.txt", DATA_SIZE); 
		f = fopen(name, "wb+");
	}

	//char s[8196];
	////s[0] = '('; s[1] = '\0';
	//s[0] = '\0';
	//for (int i = 0; i < DIMENSION; i++)
	//{
	//	if (i == DIMENSION - 1)
	//	{
	//		sprintf(s, "%s%f", s, POSITION[i]);
	//	}
	//	else
	//	{
	//		sprintf(s, "%s%f,", s, POSITION[i]);
	//	}
	//}

	fprintf(f, "INFO\tTIME_PARSER\tTIME_TRANSFER_TO_GPPGU\tTIME_TRANSFER_TO_CPU\tTIME_GPU_KERNEL_CLOCK\tTIME_GPU_KERNEL_KERNEL\tTIME_GPU_ALL\tTIME_CPU_HARDDISK\tTIME_LOAD_DATABASE\tTIME_CPU_DDRAM\tTIME_TRANSFER_DATA_TO_GPPGU\tSPEED_UP\tDATA_SIZE\tNUM_TEST\tNUM_RIGHT\tACCURACY\tTOTAL\tMIGRATION\tSTEP\tPATH_LENGTH\tPOP_SIZE\tDIMENSION\tBOUND_MIN\tBOUND_MAX\tPOSITION\tVALUE\tGRID_N\tWORKERS\tEXPECTED_VALUE\tFUNCTIONCALL\tSOMA_TYPE\tGROUP_IMMIGRATION\tOP1_THRESHOLD\tOP2_N\tTEST_FUNCTION\tFUNCTION_NAME\n");
	//fprintf(f, "%s\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\t%d\t%f\t%f\t%d\t%f\t%f\t%d\t%d\t%f\t%f\t%s\t%f\t%d\t%d\n",
		//info, TIME_PARSER, TIME_TRANSFER_TO_GPPGU, TIME_TRANSFER_TO_CPU, TIME_GPU_KERNEL_CLOCK, TIME_GPU_KERNEL_KERNEL, TIME_GPU_ALL, TIME_CPU_HARDDISK, TIME_LOAD_DATABASE, TIME_CPU_DDRAM, TIME_TRANSFER_DATA_TO_GPPGU,SPEED_UP, DATA_SIZE, NUM_TEST, NUM_RIGHT, accuracy, TIME_TOTAL,NUM_MIGRATION, (float)STEP, (float)PATH_LENGTH,POP_SIZE,DIMENSION,BOUND_X,BOUND_Y,s,VALUE,GRID_X*GRID_Y,WORLD_SIZE-1);


	//accuracy = VALUE*100.0f / (418.982880f*DIMENSION);
	fprintf(f, "%s\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\t%d\t%f\t%f\t%d\t%f\t%f\t%d\t%d\t%f\t%f\t",
		info, TIME_PARSER, TIME_TRANSFER_TO_GPPGU, TIME_TRANSFER_TO_CPU, TIME_GPU_KERNEL_CLOCK, TIME_GPU_KERNEL_KERNEL, TIME_GPU_ALL, TIME_CPU_HARDDISK, TIME_LOAD_DATABASE, TIME_CPU_DDRAM, TIME_TRANSFER_DATA_TO_GPPGU, SPEED_UP, DATA_SIZE, NUM_TEST, NUM_RIGHT, accuracy, TIME_TOTAL, NUM_MIGRATION, (float)STEP, (float)PATH_LENGTH, POP_SIZE, DIMENSION, BOUND_X, BOUND_Y);
	for (int i = 0; i < DIMENSION; i++)
	{
		if (i == DIMENSION - 1)
		{
			fprintf(f,"%f\t", POSITION[i]);
		}
		else
		{
			fprintf(f, "%f,", POSITION[i]);
		}
	}

	char op0 = 'T'; char op1 = 'T'; char op2 = 'T'; //char op3 = 'T';
	if (!OP0) op0 = 'F';
	if (!OP1) op1 = 'F';
	if (!OP2) op2 = 'F';

	fprintf(f, "%f\t%d\t%d\t%f\t%d\t%d\t%d\t%f\t%d\t%d\t%s\n", VALUE, GRID_N, WORLD_SIZE - 1,418.982880f*DIMENSION, NUM_COST_FUNCTION_CALL,SOMA_TYPE, GROUPS_PER_NODE,OP1_THRESHOLD,OP2_N, TEST_FUNCTION_ID, TestFunctions::getInstance().GetTestFunctionName());

	fclose(f);

	//convert to csv
	char name2[256];
	ifstream inFile;
	if (file_name != NULL)
	{
		inFile.open(file_name);
		sprintf(name2,"%s.csv", file_name);
	}
	else
	{
		inFile.open(name);
		sprintf(name2,"%s.csv", name);
	}
	
	if (!inFile) 
	{ 
		cerr << "Unable to open file datafile.txt\n\n\n\n";
		//exit(1);  
	}
	char c;
	ofstream of;
	of.open(name2);
	while (inFile.get(c))
	{
		if (c == '\t')
			of << ",";
		else if (c == ',')
			of << ";";
		else of << c; 
	}
	of.close();
}
#ifdef TCUDA

void TimeAnalysis::Begin_TransferToGPU()
{
	cudaEventCreate(&start_TransferToGPU); 
	cudaEventCreate(&stop_TransferToGPU);
	cudaEventRecord(start_TransferToGPU);
}
void TimeAnalysis::End_TransferToGPU()
{
	cudaEventRecord(stop_TransferToGPU);
	cudaEventSynchronize(stop_TransferToGPU);
	float tmp = 0;
	cudaEventElapsedTime(&tmp, start_TransferToGPU, stop_TransferToGPU);

	TIME_TRANSFER_TO_GPPGU += tmp;
}

void TimeAnalysis::Begin_TransferToCPU()
{
	cudaEventCreate(&start_TransferToCPU);
	cudaEventCreate(&stop_TransferToCPU);
	cudaEventRecord(start_TransferToCPU);
}
void TimeAnalysis::End_TransferToCPU()
{
	cudaEventRecord(stop_TransferToCPU);
	cudaEventSynchronize(stop_TransferToCPU);
	float tmp = 0;
	cudaEventElapsedTime(&tmp, start_TransferToCPU, stop_TransferToCPU);
	TIME_TRANSFER_TO_CPU += tmp;
}
//#include "TDef.h"
void TimeAnalysis::Begin_ProcessGPU()
{
	//if (PROCESS_ID == 0) printf("void TimeAnalysis::Begin_ProcessGPU()");
	cudaEventCreate(&start_ProcessGPU);
	cudaEventCreate(&stop_ProcessGPU);
	cudaEventRecord(start_ProcessGPU);

	start_ProcessGPU_clock = clock();
}
void TimeAnalysis::End_ProcessGPU()
{
	//if (PROCESS_ID == 0) printf("void TimeAnalysis::End_ProcessGPU()"); 
	cudaEventRecord(stop_ProcessGPU);
	cudaEventSynchronize(stop_ProcessGPU);
	float tmp = 0;
	cudaEventElapsedTime(&tmp, start_ProcessGPU, stop_ProcessGPU);

	TIME_GPU_KERNEL_KERNEL += tmp;

	end_ProcessGPU_clock = clock();
	tmp  = 1000.0f * double(end_ProcessGPU_clock - start_ProcessGPU_clock) / CLOCKS_PER_SEC;
	TIME_GPU_KERNEL_CLOCK += tmp;
}
void TimeAnalysis::Begin_TransferDatabase()
{
	cudaEventCreate(&start_TransferDatabase);
	cudaEventCreate(&end_TransferDatabase);
	cudaEventRecord(start_TransferDatabase);
}
void TimeAnalysis::End_TransferDatabase()
{
	cudaEventRecord(end_TransferDatabase);
	cudaEventSynchronize(end_TransferDatabase);
	float tmp = 0;
	cudaEventElapsedTime(&tmp, start_TransferDatabase, end_TransferDatabase);

	TIME_TRANSFER_DATA_TO_GPPGU += tmp;
}
#endif // TCUDA
void TimeAnalysis::Begin_ProcessCPU()
{
	chrono_CPU_start = std::chrono::system_clock::now();
	start_ProcessCPU_clock = clock();
}
void TimeAnalysis::End_ProcessCPU()
{
	chrono_CPU_end = std::chrono::system_clock::now();
	//auto elapsed = chrono_CPU_end - chrono_CPU_start;
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(chrono_CPU_end - chrono_CPU_start);

	end_ProcessCPU_clock = clock();
	double elapsed_secs = double(end_ProcessCPU_clock - start_ProcessCPU_clock) / CLOCKS_PER_SEC;
	float tmp = elapsed_secs*1000.0f;
	//tmp = (elapsed.count);
	//cout << "\n aaaaaaaaaaaaaa: "<< elapsed.count()*0.001f <<  "\n\n";
	tmp = elapsed.count()*0.001f;
	TIME_CPU_DDRAM += tmp;  
}
void TimeAnalysis::Begin_LoadDatabase()  
{
	start_loadDatabase = clock();
}
void TimeAnalysis::End_LoadDatabase()
{
	end_loadDatabase = clock(); 
	float tmp =  (end_loadDatabase - start_loadDatabase)*1000.0f / CLOCKS_PER_SEC;
	TIME_LOAD_DATABASE += tmp;
}

void TimeAnalysis::Begin_Parse()
{
	start_parse = clock();
}
void TimeAnalysis::End_Parse()
{
	end_parse = clock();
	float tmp = 1000.0f * double(end_parse - start_parse) / CLOCKS_PER_SEC;
	TIME_PARSER += tmp;
}
void TimeAnalysis::Begin_ProcessCPU_Disk()
{
	start_ProcessCPU_disk = clock();
}
void TimeAnalysis::End_ProcessCPU_Disk()
{
	stop_ProcessCPU_disk = clock();
	float tmp = 1000.0f * double(stop_ProcessCPU_disk - start_ProcessCPU_disk) / CLOCKS_PER_SEC;
	TIME_CPU_HARDDISK += tmp;
}

void TimeAnalysis::Begin_Process_Total()
{
	start_total = clock();
}
void TimeAnalysis::End_Process_Total()
{
	end_total = clock();
	float tmp = (end_total - start_total)*1000.0f / CLOCKS_PER_SEC;
	TIME_TOTAL += tmp; 
}
//SOMA
void TimeAnalysis::SetBound(float xmin, float xmax)
{
	BOUND_X = xmin;
	BOUND_Y = xmax; 
}
char* TimeAnalysis::BuildFilename()
{
	char op0 = 'T'; char op1 = 'T'; char op2 = 'T'; //char op3 = 'T';
	if (!OP0) op0 = 'F'; 
	if (!OP1) op1 = 'F';
	if (!OP2) op2 = 'F';
	//if (!OP3) op3 = 'F';

	char* buf = (char*)malloc(64 * sizeof(char)); 
	if (OP1)
	{
		sprintf(buf, "./RESULT/R_%d_%d_%d_%d_%d_%d_%d_%d_%d_%c%c%c_%f_%d.txt", METHOD, POP_SIZE, (int)(STEP * 100), (int)PATH_LENGTH, (int)(BOUND_X), GRID_N, DIMENSION, GROUPS_PER_NODE, SOMA_TYPE, op0, op1, op2,OP1_THRESHOLD, TEST_FUNCTION_ID);
	}
	else if (OP2)
	{
		sprintf(buf, "./RESULT/R_%d_%d_%d_%d_%d_%d_%d_%d_%d_%c%c%c_%d_%d.txt", METHOD, POP_SIZE, (int)(STEP * 100), (int)PATH_LENGTH, (int)(BOUND_X), GRID_N, DIMENSION, GROUPS_PER_NODE, SOMA_TYPE, op0, op1, op2,OP2_N, TEST_FUNCTION_ID);
	}
	else
		sprintf(buf, "./RESULT/R_%d_%d_%d_%d_%d_%d_%d_%d_%d_%c%c%c_%d.txt", METHOD, POP_SIZE, (int)(STEP * 100), (int)PATH_LENGTH, (int)(BOUND_X), GRID_N, DIMENSION, GROUPS_PER_NODE, SOMA_TYPE, op0, op1, op2, TEST_FUNCTION_ID);
	return buf;
}

#ifdef TMPI
void TimeAnalysis::SyncAllProcessor()
{
	MPI_Barrier(MPI_COMM_WORLD); 
	if (WORLD_SIZE <= 1) return;
	if (PROCESS_ID == 0)
	{
		int re = 0; 
		MPI_Status status;
		for (int i = 0; i < WORLD_SIZE - 1; i++)
		{
			re = 0; 
			MPI_Recv(&re, 1, MPI_INT, MPI_ANY_SOURCE, 92, MPI_COMM_WORLD, &status);
			NUM_COST_FUNCTION_CALL += re;
		}
		NUM_COST_FUNCTION_CALL /= (WORLD_SIZE-1);
	}
	else
	{
		MPI_Send(&NUM_COST_FUNCTION_CALL,1,MPI_INT,0,92, MPI_COMM_WORLD);
	}
}
#endif