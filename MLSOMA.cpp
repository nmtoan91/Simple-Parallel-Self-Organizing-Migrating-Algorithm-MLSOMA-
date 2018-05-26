#include "MLSOMA.h"
#include "SerializedSharedMemory.h"
#include "TimeAnalysis.h"
using namespace std;
MLSOMA::MLSOMA()
{
	is_first_try = true;

	Init_MetaInformation(); 
	
	SOMAs = new SOMA_Sequence();
	SHARED_MEMORY_size = GRID_N*(DIMENSION + 1); 
	BOUNDS_GLOBAL = (float*)malloc(DIMENSION * 2 * sizeof(float));
	BOUNDS = (float*)malloc(DIMENSION * 2 * sizeof(float));

#ifdef MPI3
	MPI_Win_create(SHARED_MEMORY, SHARED_MEMORY_size * sizeof(int), sizeof(float), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	MPI_Win_fence(0, win);
	MPI_Win_fence(0, win);
#endif // MPI3

	

	//if (PROCESS_ID == 0)
	SHARED_MEMORY = (float*)malloc(SHARED_MEMORY_size * sizeof(float));
		for (int i = 0; i < SHARED_MEMORY_size; i++)
			SHARED_MEMORY[i] = 0;
		
	
	SerializedSharedMemory::getInstance().Init();
	MPI_Barrier(MPI_COMM_WORLD);
}


MLSOMA::~MLSOMA()
{
#ifdef MPI3
	//MPI_Win_free(&win); 
#endif // MPI3
}
void MLSOMA::Run()
{
	
	if (WORLD_SIZE == 1)
	{
		Run2();
	}
	else
	{
		if (PROCESS_ID == 0) Master_Process();
		else Slave_Process();
	}
	ShowResults();
}
void MLSOMA::Run2()
{
	while (true)
	{
		int gid_id = FindAGird();
		if (gid_id == -1) break;
		SetGrid(gid_id);
		SOMAs->RanDomIndividuals();
		SOMAs->DoSOMAWithSchwefel();
		WriteResultToSharedMemory();
	}
}
void MLSOMA::Allgather()
{
	//MPI_Allgather()
}
void MLSOMA::Init_MetaInformation()
{
	GRID_N = (int)pow(GRID_X, DIMENSION);
	current_grid_index = (int*)malloc(DIMENSION*sizeof(int));
	SelfCalcGridIndex(PROCESS_ID);
}
void MLSOMA::SetBounds(float min, float max)
{
	SOMAs->SetBounds(min, max);
	for (int i = 0; i<DIMENSION; i++)
	{
		BOUNDS[i*2] = min;
		BOUNDS[i*2 + 1] = max;
	}
}
void MLSOMA::SetBounds(float xmin, float xmax, float ymin, float ymax)
{
	SOMAs->SetBounds(xmin, xmax, ymin, ymax); 
	BOUNDS[0] = xmin; 
	BOUNDS[1] = xmax; 
	BOUNDS[2] = ymin; 
	BOUNDS[3] = ymax; 
}
void MLSOMA::SetBounds(int dimension, float min, float max)
{
	BOUNDS[dimension*2] = min;
	BOUNDS[dimension*2+1] = max;
	SOMAs->SetBounds(dimension, min, max);
}
void MLSOMA::SetBoundsGlobal(float min, float max)
{
	if (WORLD_SIZE > 1 /*&& PROCESS_ID == 0*/)
	{
		TimeAnalysis::getInstance().SetBound(min, max);
	}
	for (int i = 0; i<DIMENSION; i++)
	{
		BOUNDS_GLOBAL[i*2] = min;
		BOUNDS_GLOBAL[i*2 + 1] = max;
	}
}
void MLSOMA::SelfCalcGridIndex(int index)
{
	int index2 = index;
	for (int i = 0; i < DIMENSION; i++)
	{
		current_grid_index[i] = index2%GRID_X;
		index2 = index2/ GRID_X;
	}

	/*printf("AA %d= (( ",index);
	for (int i = 0; i < DIMENSION; i++) printf("%d,", current_grid_index[i]);
	printf("))\n");*/

	//current_grid_index[0] = index%GRID_X;
	//current_grid_index[1] = index/GRID_X;
}
void MLSOMA::WriteResultToSharedMemory()
{
	int index = (current_grid_index[1] * GRID_X + current_grid_index[0])*(DIMENSION+1);
	SHARED_MEMORY[index] = SOMAs->Individuals[SOMAs->LEADER_INDEX*DIMENSION]; 
	SHARED_MEMORY[index+1] = SOMAs->Individuals[SOMAs->LEADER_INDEX*DIMENSION+1]; 
	SHARED_MEMORY[index + 2] = SOMAs->Individuals_cost[SOMAs->LEADER_INDEX]; 
}
void MLSOMA::ShowResults()
{
	 
	if (PROCESS_ID != 0) return; // only rank 0 
	
	printf("RESULTs: ");
	for (int i = 0; i < SHARED_MEMORY_size; i += DIMENSION + 1)
	{
		printf("(%f,%f, %f)    ", SHARED_MEMORY[i], SHARED_MEMORY[i + 1], SHARED_MEMORY[i + DIMENSION]);
	}
	printf("\n");
}

void MLSOMA::SetBound(int *current_grid_index)
{
#ifdef OLD
	float* step = (float*)malloc(DIMENSION * sizeof(float));
	for (int i = 0; i < DIMENSION; i++)
	{
		step[i] = (BOUNDS_GLOBAL[2 * i + 1] - BOUNDS_GLOBAL[2 * i]) / GRID_X;
	}
	float xmin = BOUNDS_GLOBAL[2 * 0] + step[0] * current_grid_index[0];
	float xmax = BOUNDS_GLOBAL[2 * 0] + step[0] * (current_grid_index[0] + 1);
	float ymin = BOUNDS_GLOBAL[2 * 1] + step[1] * current_grid_index[1];
	float ymax = BOUNDS_GLOBAL[2 * 1] + step[1] * (current_grid_index[1] + 1);
	SetBounds(xmin, xmax);
	delete step;
#else
	float* step = (float*)malloc(DIMENSION * sizeof(float));
	for (int i = 0; i < DIMENSION; i++)
	{
		step[i] = (BOUNDS_GLOBAL[2 * i + 1] - BOUNDS_GLOBAL[2 * i]) / GRID_X;
		float min = BOUNDS_GLOBAL[2 * i] + step[i] * current_grid_index[i];
		float max = BOUNDS_GLOBAL[2 * i] + step[i] * (current_grid_index[i] + 1);
		
		SetBounds(i,min, max);
	}
	delete step;
#endif // OLD

	
}
bool MLSOMA::IsGirdCompleted()
{
	if (SerializedSharedMemory::getInstance().GetCurrentGrid() > GRID_N - 1)
		return true; 
	return false;
}
void MLSOMA::SetGrid(int id)
{
	if (GRID_N < 100)
	{
		printf("PROCESS %d check GriD %d \n", PROCESS_ID, id);
	}
	SelfCalcGridIndex(id);
	SetBound(current_grid_index);
}
bool MLSOMA::IsGridDone(int id)
{
	int s = DIMENSION + 1; 
	int index = id*s;

	if (SHARED_MEMORY[id*s] == SHARED_MEMORY[id*s + 1] &&
		SHARED_MEMORY[id*s + 2] == SHARED_MEMORY[id*s + 1]) return false;
	return true;
}
int MLSOMA::FindAGird() 
{
	for (int i = 0; i < GRID_N; i++)
	{
		if (!IsGridDone(i)) return i;
	}
	return -1;
}

void MLSOMA::Master_Process()
{
	int buf_size = (DIMENSION + 1 + 1);
	float * buf = (float*)malloc(buf_size * sizeof(float));
	MPI_Status status;
	int num_receieve = 0; 

	TimeAnalysis::getInstance().Begin_ProcessCPU();
	// set auto
	for (int i = 0; i < WORLD_SIZE-1; i++)
	{
		SHARED_MEMORY[i*(DIMENSION + 1)] = -2;
	}
	while (true)
	{
		
		MPI_Recv(buf, buf_size, MPI_FLOAT, MPI_ANY_SOURCE, 11, MPI_COMM_WORLD, &status);
		int index = ((int)buf[0])*(DIMENSION + 1);
		for (int i = 0; i <= DIMENSION; i++)
		{
			SHARED_MEMORY[index + i] = buf[1 + i];
		}
		int next_grid = FindAGird();
		buf[0] = next_grid;

		if (next_grid >= 0)
		{
			SHARED_MEMORY[next_grid*(DIMENSION + 1)] = -2;
		}

		MPI_Send(buf, 1, MPI_FLOAT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
		num_receieve++;
		if (num_receieve >= GRID_N) break;
	}
	ConclueResults();
	for (int d = 0; d < DIMENSION; d++)
	{
		TimeAnalysis::getInstance().POSITION[d] = GLOBAL_POSITION[d];
	}
	TimeAnalysis::getInstance().VALUE = GLOBAL_VALUE;

	TimeAnalysis::getInstance().End_ProcessCPU();
}
void MLSOMA::Slave_Process()
{
	int gid_id = -1;
	while (true)
	{
		if (is_first_try)
		{
			is_first_try = false;
			gid_id = MY_DEVICE_ID;
		}
		if (gid_id == -1) break;

		SetGrid(gid_id);

		SOMAs->RanDomIndividuals();
		SOMAs->DoSOMAWithSchwefel();
		WriteResultToSharedMemory();

		int buf_size = (DIMENSION + 1 + 1);
		float * buf = (float*)malloc(buf_size * sizeof(float));
		int index = (current_grid_index[1] * GRID_X + current_grid_index[0])*(DIMENSION + 1);


		buf[0] = gid_id;
		for (int i = 0; i < DIMENSION; i++)
		{
			buf[1 + i] = SOMAs->Individuals[SOMAs->LEADER_INDEX*DIMENSION +i];
		}
		buf[DIMENSION+1] = SOMAs->Individuals_cost[SOMAs->LEADER_INDEX];


		MPI_Send(buf, buf_size, MPI_FLOAT, 0, 11, MPI_COMM_WORLD);

		MPI_Status status;
		MPI_Recv(buf, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
		if (buf[0] < 0) break;
		else gid_id = (int)buf[0];
	}
}

void MLSOMA::ConclueResults()
{
	int index_max = 0 ;
	float value_max = SHARED_MEMORY[DIMENSION];
	for (int i = 0; i < GRID_N; i++)
	{
		int value = SHARED_MEMORY[i*(DIMENSION + 1) + (DIMENSION)];
		if (value > value_max)
		{
			index_max = i;
			value_max = value;
		}
	}
	GLOBAL_POSITION = (float*)malloc(DIMENSION * sizeof(float));
	for (int i = 0; i < DIMENSION; i++)
	{
		GLOBAL_POSITION[i] = SHARED_MEMORY[index_max*(DIMENSION + 1) + i];
	}
	GLOBAL_VALUE = value_max;
}