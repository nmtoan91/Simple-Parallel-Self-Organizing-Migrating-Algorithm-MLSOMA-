#pragma once
#define TMPI
#ifdef TCUDA
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#endif // TCUDA
#include <time.h>
#include <chrono>
class TimeAnalysis
{
public:
	//variables
	float TIME_PARSER; //0
	float TIME_TRANSFER_TO_GPPGU; //1
	float TIME_TRANSFER_TO_CPU; //2
	float TIME_GPU_KERNEL_CLOCK; //3
	float TIME_GPU_KERNEL_KERNEL; //4
	float TIME_GPU_ALL;//5
	float TIME_CPU_HARDDISK; //6 
	
	float TIME_LOAD_DATABASE; //7
	float TIME_CPU_DDRAM; //8
	float TIME_TRANSFER_DATA_TO_GPPGU; //9 
	float SPEED_UP; //10
	float DATA_SIZE; //
	int NUM_TEST; 
	int NUM_RIGHT; 
	float TIME_TOTAL; 
	float BOUND_X;
	float BOUND_Y;
	int NUM_COST_FUNCTION_CALL;
	//SOMA
	int NUM_MIGRATION;
	//functions
	 TimeAnalysis();
	~TimeAnalysis();
	 void Init(); 
	 void WriteResult(char* file_name, char* info = (char*)"N/A");
	 static TimeAnalysis& getInstance(){static TimeAnalysis    instance; return instance;}
	 void Begin_TransferToGPU();
	 void End_TransferToGPU();
	 void Begin_TransferToCPU();
	 void End_TransferToCPU();
	 void Begin_ProcessGPU();
	 void End_ProcessGPU();
	 void Begin_ProcessCPU();
	 void End_ProcessCPU();
	 void Begin_LoadDatabase();
	 void End_LoadDatabase();
	 void Begin_TransferDatabase();
	 void End_TransferDatabase();
	 void Begin_Parse();
	 void End_Parse();
	 void Begin_ProcessCPU_Disk();
	 void End_ProcessCPU_Disk();
	 void Begin_Process_Total();
	 void End_Process_Total(); 
	 //SOMA
	 void SetBound(float xmin, float xmax);
	 //float X, Y;
	 float *POSITION;
	 float VALUE;
	 char* BuildFilename();
private:
	
#ifdef TCUDA
	cudaEvent_t start_TransferToGPU, stop_TransferToGPU;
	cudaEvent_t start_TransferToCPU, stop_TransferToCPU;
	cudaEvent_t start_ProcessGPU, stop_ProcessGPU;
	cudaEvent_t start_TransferDatabase, end_TransferDatabase;
#endif // TCUDA
	clock_t start_ProcessGPU_clock, end_ProcessGPU_clock;
	clock_t start_ProcessCPU_disk, stop_ProcessCPU_disk; 
	clock_t start_ProcessCPU_clock, end_ProcessCPU_clock;
	clock_t start_loadDatabase, end_loadDatabase;
	clock_t start_parse, end_parse;
	clock_t start_total, end_total;
	std::chrono::time_point<std::chrono::system_clock> chrono_CPU_start;
	std::chrono::time_point<std::chrono::system_clock> chrono_CPU_end;
#ifdef TMPI
public:
	void SyncAllProcessor(); 
#endif
};

