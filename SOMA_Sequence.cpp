#include "SOMA_Sequence.h"
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include "TimeAnalysis.h"
#if __linux__
#else
#include <math.h>
#endif

#include <iostream>
#include <cstdint>
#include <cstring>

using namespace std;
SOMA_Sequence::SOMA_Sequence(void)
{
	OP0_THRESHOLD = RANGE*0.0001f;

	f.SetTestfunction2(TEST_FUNCTION_ID);
	Individuals_cost = (float*)malloc(POP_SIZE * sizeof(float));
	Individuals_status = (bool*)malloc(POP_SIZE * sizeof(bool));
	Individuals_isChanged = (bool*)malloc(POP_SIZE * sizeof(bool));
	for (int i = 0; i < POP_SIZE; i++)
	{
		Individuals_status[i] = true;
		Individuals_isChanged = false;
	}

	Individuals = (float*)malloc(POP_SIZE *DIMENSION* sizeof(float));
	PRT_Vectors = (float*)malloc(POP_SIZE *DIMENSION * sizeof(float));
	BOUNDS = (float*)malloc(2 *DIMENSION * sizeof(float)); 
} 
SOMA_Sequence::~SOMA_Sequence(void)
{

}
void SOMA_Sequence::RanDomIndividuals()
{
	//f.SetTestfunction2(TEST_FUNCTION_ID);

	Clear();
	srand(time(NULL)+PROCESS_ID);
	for(int i =0;i<POP_SIZE;i++)
	{
		TVector vec_temp;
		for(int j =0;j<DIMENSION;j++)
		{
			Individuals[i*DIMENSION+j] = ((rand() % 10000)/10000.0f)*(BOUNDS[2*j+1] - BOUNDS[2*j]) + BOUNDS[2*j];
		}
	}
}
void SOMA_Sequence::RanDomIndividuals(float *target, float factor)
{
	Clear();
	float step;
	srand(time(NULL) + PROCESS_ID);
	for (int i = 0; i<POP_SIZE; i++)
	{
		TVector vec_temp;
		for (int j = 0; j<DIMENSION; j++)
		{
			step = (BOUNDS[2 * j + 1] - BOUNDS[2 * j])*factor;

			//Individuals[i*DIMENSION + j] = ((rand() % 10000) / 10000.0f)*(BOUNDS[2 * j + 1] - BOUNDS[2 * j]) + BOUNDS[2 * j];
			Individuals[i*DIMENSION + j] = target[j] + ((rand() % 10000) / 10000.0f - 0.5f)*step;
			if (Individuals[i*DIMENSION + j] > BOUNDS[2 * j + 1]) Individuals[i*DIMENSION + j] = BOUNDS[2 * j + 1];
			else if (Individuals[i*DIMENSION + j] < BOUNDS[2 * j ]) Individuals[i*DIMENSION + j] = BOUNDS[2 * j ];
		}
	}
}
void SOMA_Sequence::FindLeader()
{
	//float min_cost = f.Schwefel_(&Individuals[LEADER_INDEX*DIMENSION]);
	float min_cost = f.testfunction_(&Individuals[LEADER_INDEX*DIMENSION]);
	LEADER_INDEX = LEADER_INDEX;
	for(int i =0;i<POP_SIZE;i++)
	{
		//float temp = f.Schwefel_(&Individuals[i*DIMENSION]);
		float temp = f.testfunction_(&Individuals[i*DIMENSION]);
		Individuals_cost[i] = temp;
		//if(temp < min_cost)
		if (temp > min_cost)
		{
			min_cost = temp;
			LEADER_INDEX=i;
		}

		if (OP0 && Individuals_status[i])
		{
			float d = 0; 
			for (int j = i + 1; j < POP_SIZE; j++)
			{
				if (Individuals_status[j])
				{
					d = 0;
					for (int d = 0; d < DIMENSION; d++)
						d += fabs(Individuals[i*DIMENSION+d] - Individuals[j*DIMENSION + d]);
					if (d < OP0_THRESHOLD)
					{
						Individuals_status[j] = false;
						break;
					}
				}
			}
		}
	}
	if(last_leader_cost != min_cost)
	{
		if (MY_DEVICE_ID == 0 && IS_WRITE_MIGRATIONS)
		{
			TOutput::getInstance().WriteMigration(Individuals, Individuals_cost, LEADER_INDEX);
		}
		last_leader_cost = min_cost;
	}

	
}
TVector SOMA_Sequence::Get_PRT_Vectors()
{
	TVector tvector_temp;
	for(int j =0;j<DIMENSION;j++)
	{
		if(rand()/32767.0f <PRT_THRESHOLD)
			tvector_temp.A[j] =1;
	}
	return tvector_temp;
}
float* SOMA_Sequence::Get_PRT_Vectors_(float *f)
{
	bool is_all_zero = true;
	for (int j = 0; j<DIMENSION; j++)
	{
		if (rand() / 32767.0f < PRT_THRESHOLD)
		{
			f[j] = 1;
			is_all_zero = false;
		}
		else f[j] = 0;
	}
	if (is_all_zero)
		f[((int)rand()) % DIMENSION] = 1;
	return f;
}
bool SOMA_Sequence::Is_InBounds(TVector in)
{
	for(int j =0;j<DIMENSION;j++)
	{
		if (in.A[j] <BOUNDS[j]) return false;
		if(in.A[j] >BOUNDS[j+1]) return false;
	}
	return true;
}
bool SOMA_Sequence::Is_InBounds(float* in)
{
	//if (in[0] * in[1] < 0)
		//int asd = 23;
	for (int j = 0; j<DIMENSION; j++)
	{
		if (in[j] <BOUNDS[j*2]) return false;
		if (in[j] >BOUNDS[j*2 + 1]) return false;
	}
	return true;
}
void SOMA_Sequence::DoSOMAWithSchwefel()
{
	if (WORLD_SIZE == 1) TimeAnalysis::getInstance().Begin_ProcessCPU();

	LEADER_INDEX=0;
	FindLeader();
	
	bool is_have_change = false;
	float* current_vec = (float*)malloc(DIMENSION * sizeof(float));
	float* current_leader_temp;
	float* Individuals_J_temp = (float*)malloc(DIMENSION * sizeof(float));

	int j_leader_count = 0;
	for(int i =0;i<MIGRATIONS;i++)
	{
		is_have_change = false;
		for(int j =0;j<POP_SIZE;j++)
		{
			j_leader_count = 0;
			if (OP0 && !Individuals_status[j]) continue;
			
			float cost_temp = 0;
			Get_PRT_Vectors_(&(PRT_Vectors[j*DIMENSION]));
			
			float Individuals_cost_min = Individuals_cost[j];
			
			std::memcpy(Individuals_J_temp,&Individuals[j*DIMENSION],DIMENSION*sizeof(float));

			if (SOMA_TYPE == 0) // ALl to Leader
			{
				current_leader_temp = &Individuals[LEADER_INDEX*DIMENSION];
				RunSOMAPath(j, current_leader_temp, current_vec, &is_have_change, Individuals_J_temp, &Individuals_cost_min);
			}
			// j to t
			else if(SOMA_TYPE ==1) // All to All
			{
				for (int t = 0; t < POP_SIZE; t++)
				{
					if (t == j) continue;
					if (OP0 && !Individuals_status[t]) continue;
					if (OP1 && Individuals_cost[j] > Individuals_cost[t] - OP1_THRESHOLD) continue;
					if (OP2)
					{ 
						if (Individuals_cost[j] > Individuals_cost[t] - OP1_THRESHOLD) continue; 
						if (j_leader_count > OP2_N) continue; 
						j_leader_count++; 
					}
					current_leader_temp = &Individuals[t*DIMENSION];
					RunSOMAPath(j, current_leader_temp, current_vec, &is_have_change, Individuals_J_temp, &Individuals_cost_min);
				}
			}
			memcpy(&Individuals[j*DIMENSION] , Individuals_J_temp, DIMENSION*sizeof(float));
			Individuals_cost[j] = Individuals_cost_min;
		}
		FindLeader();
		if (!is_have_change) 
		{
			if (WORLD_SIZE == 1) 
			{
				printf("break at mirgration: %d \n", i);
				printf("result for me: (%f,%f,%f,%f) -> (%f,%f,%f)\n", BOUNDS[0], BOUNDS[1], BOUNDS[2], BOUNDS[3], Individuals[LEADER_INDEX*DIMENSION], Individuals[LEADER_INDEX*DIMENSION + 1], Individuals_cost[LEADER_INDEX]);
				TimeAnalysis::getInstance().NUM_MIGRATION = i;
				for (int d = 0; d < DIMENSION; d++)
				{
					TimeAnalysis::getInstance().POSITION[d] = Individuals[LEADER_INDEX*DIMENSION+d];
				}
				TimeAnalysis::getInstance().VALUE = Individuals_cost[LEADER_INDEX];
			}
			break; 
		}
	}

	TimeAnalysis::getInstance().End_ProcessCPU();
}
float* SOMA_Sequence::RunSOMAPath(int index,float *target, float* current_vec,  bool *is_have_change,float *Individuals_J_temp, float* Individuals_cost_min)
{
	float cost_temp = 0;
	//float Individuals_cost_min = Individuals_cost[index];

	for (float t = STEP; t <= PATH_LENGTH; t += STEP)
	{
		for (int d = 0; d < DIMENSION; d++)
		{
			current_vec[d] = Individuals[index*DIMENSION + d]
				+ (target[d] - Individuals[index*DIMENSION + d])
				*t*PRT_Vectors[index*DIMENSION + d];
		}
		if (!Is_InBounds(current_vec)) continue;

		//cost_temp = f.Schwefel_(current_vec);
		cost_temp = f.testfunction_(current_vec);

		if (cost_temp>(*Individuals_cost_min))
		{
			*is_have_change = true;
			for (int d = 0; d < DIMENSION; d++)
			{
				Individuals_J_temp[d] = current_vec[d];
			}
			(*Individuals_cost_min) = cost_temp;
		}
	}
	return NULL;
}
void SOMA_Sequence::Test_PrintIndividuals()
{
	printf("Individuals: ");
	for (int i = 0; i<POP_SIZE; i+= DIMENSION)
	{
		printf(" (%f,%f) ", Individuals[i], Individuals[i+1]);
	}
	printf("\n");
}
void SOMA_Sequence::Test_PrintBounds()
{
	printf("Bounds: ");
	for (int i = 0; i<DIMENSION; i ++)
	{
		printf(" (%f,%f) ", BOUNDS[2*i], BOUNDS[2 * i+1]);
	}
	printf("\n");
}
void SOMA_Sequence::SetBounds(float min, float max)
{
	for (int i = 0; i<DIMENSION; i++)
	{
		BOUNDS[i*2] = min;
		BOUNDS[i*2 +1] = max;
	}
	if (WORLD_SIZE == 1)
	{
		TimeAnalysis::getInstance().SetBound(min, max);
	}
}
void SOMA_Sequence::SetBounds(float *values)
{
	SetBounds(values[0], values[1]);
}

void SOMA_Sequence::SetBounds(float xmin, float xmax, float ymin, float ymax)
{
	//SOMAs->SetBounds(xmin, xmax, ymin, ymax); 
	BOUNDS[0] = xmin;
	BOUNDS[1] = xmax;
	BOUNDS[2] = ymin;
	BOUNDS[3] = ymax;
	//printf("setting for me: (%f,%f,%f,%f)\n", BOUNDS[0], BOUNDS[1], BOUNDS[2], BOUNDS[3]);
}
void SOMA_Sequence::SetBounds(int dimension, float min, float max)
{
	BOUNDS[dimension*2] = min;
	BOUNDS[dimension*2+1] = max;
}

void SOMA_Sequence::Clear()
{
	for (int i = 0; i < POP_SIZE; i++)
	{
		Individuals_status[i] = true;
		Individuals_cost[i] = 0;
	}
	last_leader_cost = 0; 
	LEADER_INDEX = 0;
}

//void SOMA_Sequence::InitParameters(int argc, char *argv[])
//{
//	int i = 1;
//	while (i < argc)
//	{
//		if (strcmp(argv[i], "-pop") == 0)
//		{
//			POP_SIZE = atoi(argv[++i]);
//		}
//		else if (strcmp(argv[i], "-step") == 0)
//		{
//			STEP = atof(argv[++i]);
//		}
//		else if (strcmp(argv[i], "-plength") == 0)
//		{
//			PATH_LENGTH = atof(argv[++i]);
//		}
//		else if (strcmp(argv[i], "-range") == 0)
//		{
//			RANGE = atof(argv[++i]);
//		}
//		else if (strcmp(argv[i], "-d") == 0)
//		{
//			DIMENSION = atoi(argv[++i]);
//		}
//		else if (strcmp(argv[i], "-grid") == 0)
//		{
//			GRID_X = GRID_Y = atoi(argv[++i]);
//		}
//		i++;
//	}
//}