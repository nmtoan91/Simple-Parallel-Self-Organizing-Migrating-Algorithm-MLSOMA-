#include "SerializedSharedMemory.h"




SerializedSharedMemory::SerializedSharedMemory()
{
	
}
void SerializedSharedMemory::Init()
{
#ifdef MPI_3
	int size = 1;
	int SHARED_MEMORY_size = size;
	SHARED_MEMORY = (float*)malloc(SHARED_MEMORY_size * sizeof(float));

	if (PROCESS_ID == 0)
	{
		MPI_Win_allocate_shared(SHARED_MEMORY_size * sizeof(float), sizeof(float), MPI_INFO_NULL,
			MPI_COMM_WORLD, &SHARED_MEMORY, &win);
	}
	else
	{
		int disp_unit;
		MPI_Aint ssize;
		MPI_Win_allocate_shared(0, sizeof(float), MPI_INFO_NULL,
			MPI_COMM_WORLD, &SHARED_MEMORY, &win);
		MPI_Win_shared_query(win, 0, &ssize, &disp_unit, &SHARED_MEMORY);
	}
	MPI_Barrier(MPI_COMM_WORLD); 
	if (PROCESS_ID == 0)
	{
		SetCurrentGrid(0);  
	}
#endif
}


SerializedSharedMemory::~SerializedSharedMemory()
{

}

int SerializedSharedMemory::GetCurrentGrid() // float 0
{
	return (int)SHARED_MEMORY[0];
} 
bool SerializedSharedMemory::SetCurrentGrid(int id)
{
#ifdef MPI_3
	MPI_Win_lock(MPI_LOCK_EXCLUSIVE, PROCESS_ID, MPI_MODE_NOCHECK, win);
	SHARED_MEMORY[0] = id;
	MPI_Win_unlock(PROCESS_ID, win);
#endif
	return true;
}

