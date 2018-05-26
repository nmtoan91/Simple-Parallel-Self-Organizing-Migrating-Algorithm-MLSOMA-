#pragma once
#include "mpi.h"
#include "TDefine.h"
class SerializedSharedMemory
{
private:
	int size; 
	float* SHARED_MEMORY;
	MPI_Win win;
public:
	SerializedSharedMemory(); 
	~SerializedSharedMemory();
	static SerializedSharedMemory& getInstance() { static SerializedSharedMemory    instance; return instance; }
	void Init();
	int GetCurrentGrid(); // float 0
	bool SetCurrentGrid(int);
};

