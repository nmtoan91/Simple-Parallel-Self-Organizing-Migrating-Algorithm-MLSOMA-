#pragma once
#include "TVector.h"
#include "TDefine.h"
class TestFunctions
{
public: 
	float(*testfunction_)(float* in);
	float(*testfunction)(TVector in);
	TestFunctions(void);
	~TestFunctions(void);
	static float A(float c);
	static float Schwefel(TVector input);
	static float DeJong(TVector input);
	static float Schwefel2(TVector input);
	static float Schwefel_(float*input);
	void SetTestfunction(int index);
	static float Rosenbrock(TVector input);
	static float DeJong3(TVector input);
	static float DeJong4(TVector input);
	static float Rastrigin(TVector input);
	static float Griewangk(TVector input);
	static float StretchedV(TVector input);
	static float Ackley(TVector input);
	static float EggHolder(TVector input);
	static float Rana(TVector input);
	static float Pathological(TVector input);
	static float Michalewicz(TVector input);
	static float Masters_InvertedCosine(TVector input);
	//float array 20180411
	static float DeJong(float* input);
	static float Rosenbrock(float* input);
	static float DeJong3(float* input);
	static float DeJong4(float* input);
	static float Rastrigin(float* input);
	static float Griewangk(float* input);
	static float StretchedV(float* input);
	static float Ackley(float* input);
	static float EggHolder(float* input);
	static float Rana(float* input);
	static float Pathological(float* input);
	static float Michalewicz(float* input);
	static float Masters_InvertedCosine(float* input);

	void SetTestfunction2(int index);
	char* GetTestFunctionName();
	static TestFunctions& getInstance() { static TestFunctions    instance; return instance; }
};

