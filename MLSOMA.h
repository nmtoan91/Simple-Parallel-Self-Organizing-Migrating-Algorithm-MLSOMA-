#pragma once
#include "SOMA_Sequence.h"
#include "mpi.h"


class MLSOMA
{
public:
	SOMA_Sequence* SOMAs; 
	float* SHARED_MEMORY; 
	int SHARED_MEMORY_size; 
	//int GRID_N;
	float *BOUNDS; 
	float *BOUNDS_GLOBAL;
	int *current_grid_index; 
	MPI_Win win; 
	bool is_first_try;
	float* GLOBAL_POSITION;
	float GLOBAL_VALUE;
	//bool IsGirdCompleted_; 
public:
	MLSOMA();
	~MLSOMA();
	void Run();
	void Run2();
	void Allgather();
	void SetBounds(float min, float max);
	void SetBounds(float xmin, float xmax, float ymin, float ymax);
	void SetBounds(int dimension,float min, float max);
	void SetBoundsGlobal(float min, float max);
	void ShowResults();
private:
	void Init_MetaInformation();
	void SelfCalcGridIndex(int index);
	void WriteResultToSharedMemory();
	void SetBound(int *current_grid_index);
	bool IsGirdCompleted();
	void SetGrid(int);
	bool IsGridDone(int id);
	int FindAGird();

	void Master_Process();
	void Slave_Process();
	void ConclueResults();
};