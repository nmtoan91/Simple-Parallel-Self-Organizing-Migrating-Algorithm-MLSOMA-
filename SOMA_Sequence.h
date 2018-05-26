#pragma once
#include "TDefine.h"
#include "TVector.h"
#include "TestFunctions.h"
#include "TOutput.h"
class SOMA_Sequence
{
public:
	float last_leader_cost;
	TestFunctions f;
	int LEADER_INDEX;
	float *Individuals_cost;
	float *Individuals;// [POP_SIZE*DIMENSION];
	float *PRT_Vectors;// [POP_SIZE*DIMENSION];
	float *BOUNDS;// [DIMENSION * 2];
	bool *Individuals_status;
	bool *Individuals_isChanged;
	float(*testfunction)(TVector in);
	float OP0_THRESHOLD;
public:
	SOMA_Sequence(void);
	~SOMA_Sequence(void);
	void RanDomIndividuals();
	void RanDomIndividuals(float *target, float factor=0.25f);
	void DoSOMAWithSchwefel();
	void FindLeader();
	TVector Get_PRT_Vectors();
	float* Get_PRT_Vectors_(float *f);
	bool Is_InBounds(TVector in);
	bool Is_InBounds(float* in);
	void Test_PrintIndividuals();
	void Test_PrintBounds();
	void SetBounds(float min, float max);
	void SetBounds(float *values);
	void SetBounds(float xmin, float xmax, float ymin, float ymax);
	void SetBounds(int dimension,float min, float max);
	void Clear();
	float* RunSOMAPath(int index,float *target, float* current_vec,bool *is_have_change,float *Individuals_J_temp, float* Individuals_cost_min);
	//MPI
	//static void InitParameters(int argc, char *argv[]);
};

