#pragma once
#include "SOMA_Sequence.h"
#include "mpi.h"
#include "TimeAnalysis.h"
#include "SOMAMPIMessage.h"
class MLSOMA2
{
public:
	SOMA_Sequence* SOMAs;
	float* SHARED_MEMORY;
	int SHARED_MEMORY_size;
	float *BOUNDS;
	float* GLOBAL_POSITION;
	float GLOBAL_VALUE;
	SOMAMPIMessage * MPIMESSAGE;
public:
	MLSOMA2();
	~MLSOMA2();
	void SetBounds(float min, float max);
	void Run();
private:
	void Init_MetaInformation();
	void WriteResultToSharedMemory();

	void Master_Process();
	void Slave_Process();
	
	void Run2();
	void ConclueResults();
	void ShowResults();
};

