#include "MLSOMA2.h"



MLSOMA2::MLSOMA2()
{
	Init_MetaInformation();
	SOMAs = new SOMA_Sequence();
	SHARED_MEMORY_size = (WORLD_SIZE - 1)*(DIMENSION + 1);
	BOUNDS = (float*)malloc(DIMENSION * 2 * sizeof(float));
	GLOBAL_POSITION = (float*)malloc(DIMENSION * sizeof(float));
	SHARED_MEMORY = (float*)malloc(SHARED_MEMORY_size * sizeof(float));
	for (int i = 0; i < SHARED_MEMORY_size; i++)
		SHARED_MEMORY[i] = 0;
	MPI_Barrier(MPI_COMM_WORLD);
	GLOBAL_VALUE = -1;
}


MLSOMA2::~MLSOMA2()
{
}
void MLSOMA2::Init_MetaInformation()
{
	GRID_N = (int)pow(GRID_X, DIMENSION);
	//current_grid_index = (int*)malloc(DIMENSION * sizeof(int));
	//SelfCalcGridIndex(PROCESS_ID);
}
void MLSOMA2::SetBounds(float min, float max)
{
	SOMAs->SetBounds(min, max);
	for (int i = 0; i < DIMENSION; i++)
	{
		BOUNDS[i * 2] = min;
		BOUNDS[i * 2 + 1] = max;
	}
}
void MLSOMA2::WriteResultToSharedMemory()
{
	//int index = (current_grid_index[1] * GRID_X + current_grid_index[0])*(DIMENSION + 1);
	//SHARED_MEMORY[index] = SOMAs->Individuals[SOMAs->LEADER_INDEX*DIMENSION];
	//SHARED_MEMORY[index + 1] = SOMAs->Individuals[SOMAs->LEADER_INDEX*DIMENSION + 1];
	//SHARED_MEMORY[index + 2] = SOMAs->Individuals_cost[SOMAs->LEADER_INDEX];
}



void MLSOMA2::ConclueResults()
{
	int index_max = 0;
	float value_max = SHARED_MEMORY[DIMENSION];
	for (int i = 0; i < WORLD_SIZE - 1; i++)
	{
		int value = SHARED_MEMORY[i*(DIMENSION + 1) + (DIMENSION)];
		if (value > value_max)
		{
			index_max = i;
			value_max = value;
		}
	}

	/*
	for (int i = 0; i < DIMENSION; i++)
	{
		GLOBAL_POSITION[i] = SHARED_MEMORY[index_max*(DIMENSION + 1) + i];
	}
	GLOBAL_VALUE = value_max;*/

	//printf("global: %f \n ", GLOBAL_VALUE);
}

void MLSOMA2::Run()
{

	if (WORLD_SIZE == 1)
	{
		Run2();
	}
	else
	{
		if (PROCESS_ID == 0)
			Master_Process();
		else
			Slave_Process();
	}
	ShowResults();
}
void MLSOMA2::Master_Process()
{
	int buf_size = (DIMENSION + 1 + 1);
	float * buf = (float*)malloc(buf_size * sizeof(float));
	MPI_Status status;
	int num_receieve = 0;
	TimeAnalysis::getInstance().Begin_ProcessCPU();
	for (int i = 0; i < WORLD_SIZE - 1; i++)
	{
		SHARED_MEMORY[i*(DIMENSION + 1)] = -2;
	}
	while (true)
	{
		MPI_Recv(buf, buf_size, MPI_FLOAT, MPI_ANY_SOURCE, 11, MPI_COMM_WORLD, &status);
		int index = ((int)buf[0])*(DIMENSION + 1);

		if (GLOBAL_VALUE < buf[1 + DIMENSION])
		{
			for (int i = 0; i <= DIMENSION; i++)
				GLOBAL_POSITION[i] = buf[1 + i];
			GLOBAL_VALUE = buf[1 + DIMENSION];
		}

		if (SHARED_MEMORY[index + DIMENSION] < buf[1 + DIMENSION])
		{
			for (int i = 0; i <= DIMENSION; i++)
			{
				SHARED_MEMORY[index + i] = buf[1 + i];
			}
		}



		MPI_Send(GLOBAL_POSITION, DIMENSION, MPI_FLOAT, status.MPI_SOURCE, 12, MPI_COMM_WORLD);

		num_receieve++;
		if (num_receieve >= (WORLD_SIZE - 1)*GROUPS_PER_NODE) break;

	}
	ConclueResults();
	for (int d = 0; d < DIMENSION; d++)
	{
		TimeAnalysis::getInstance().POSITION[d] = GLOBAL_POSITION[d];
	}
	TimeAnalysis::getInstance().VALUE = GLOBAL_VALUE;
	TimeAnalysis::getInstance().End_ProcessCPU();

	TimeAnalysis::getInstance().SyncAllProcessor();
}
void MLSOMA2::Slave_Process()
{
	MPI_Status status;
	int gid_id = -1;
	//while (true)
	bool random = true;

	float* GLOBAL_POSITION_target = (float*)malloc(DIMENSION * sizeof(float));

	for(int g = 0 ; g < GROUPS_PER_NODE; g ++ )
	{
		if (random)
		{
			SOMAs->RanDomIndividuals();
			random = false;
		}
		else
		{
			for (int i = 0; i < DIMENSION; i++)
			{
				GLOBAL_POSITION_target[i] = SOMAs->Individuals[SOMAs->LEADER_INDEX*DIMENSION + i] * 0.75f
					+ GLOBAL_POSITION[i] * 0.25f; 
			}
			SOMAs->RanDomIndividuals(GLOBAL_POSITION_target);
		}
		SOMAs->DoSOMAWithSchwefel();
		int buf_size = (DIMENSION + 1 + 1);
		float * buf = (float*)malloc(buf_size * sizeof(float));
		int index = MY_DEVICE_ID;
		buf[0] = index;
		for (int i = 0; i < DIMENSION; i++)
		{
			buf[1 + i] = SOMAs->Individuals[SOMAs->LEADER_INDEX*DIMENSION + i];
		}
		buf[DIMENSION + 1] = SOMAs->Individuals_cost[SOMAs->LEADER_INDEX];
		MPI_Send(buf, buf_size, MPI_FLOAT, 0, 11, MPI_COMM_WORLD);
		MPI_Recv(GLOBAL_POSITION, DIMENSION, MPI_FLOAT, 0, 12, MPI_COMM_WORLD, &status);
	}

	TimeAnalysis::getInstance().SyncAllProcessor();
}
void MLSOMA2::Run2()
{
	SOMAs->RanDomIndividuals();
	SOMAs->DoSOMAWithSchwefel();
	WriteResultToSharedMemory();
}
void MLSOMA2::ShowResults()
{

	if (PROCESS_ID != 0) return; // only rank 0 

	printf("RESULTs: ");
	for (int i = 0; i < SHARED_MEMORY_size; i += DIMENSION + 1)
	{
		printf("(%f,%f, %f)    ", SHARED_MEMORY[i], SHARED_MEMORY[i + 1], SHARED_MEMORY[i + DIMENSION]);
	}
	printf("\n");
}
