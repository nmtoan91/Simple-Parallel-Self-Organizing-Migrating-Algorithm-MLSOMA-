#include "TestFunctions.h"
#include <math.h>
#include "TimeAnalysis.h"
TestFunctions::TestFunctions(void)
{
}


TestFunctions::~TestFunctions(void)
{
}
float TestFunctions::A(float c)
{
	return 0;
}
float TestFunctions::Schwefel(TVector input)
{
	TimeAnalysis::getInstance().NUM_COST_FUNCTION_CALL++;
	return -input.A[0]*sin(sqrt(fabs(input.A[0])))
		- input.A[1]*sin(sqrt(fabs(input.A[1]))); 
}
float TestFunctions::Schwefel2(TVector input)
{
	TimeAnalysis::getInstance().NUM_COST_FUNCTION_CALL++;
	return -input.A[0] * sin(sqrt(fabs(input.A[0])))
		- input.A[1] * sin(sqrt(fabs(input.A[1])));
}
float TestFunctions::Schwefel_(float*input)
{
	TimeAnalysis::getInstance().NUM_COST_FUNCTION_CALL++;
	float v = 0;
	for (int d = 0; d < DIMENSION; d++)
	{
		v -= input[d] * sin(sqrt(fabs(input[d])));
	}
	return v;
}
void TestFunctions::SetTestfunction(int index)
{
	switch (index)
	{
	case 0: testfunction = &DeJong; break;
	case 1: testfunction = &Schwefel; break;
	
	default:
		testfunction = &Schwefel;
		break;
	}
	
}

//float array 20180411
float TestFunctions::DeJong(float* input)
{
	//return input.A[0] * input.A[0] + input.A[1] * input.A[1]; 
	float v = 0;
	for (int d = 0; d < DIMENSION; d++)
	{
		v += input[d] * input[d];
	}
	return v;
}
float TestFunctions::Rosenbrock(float* input)
{
	//return 100 * (input.A[0] * input.A[0] - input.A[1] * input.A[1]) + (1 - input.A[0] * input.A[0])*(1 - input.A[0] * input.A[0]);
	float v = 0;
	for (int d = 0; d < DIMENSION-1; d++)
	{
		v += 100 * (input[0] * input[0] - input[1] * input[1])
			+ (1 - input[0] * input[0])*(1 - input[0] * input[0]);
	}
	return v;
}
float TestFunctions::DeJong3(float* input)
{
	//return fabs(input.A[0]) + fabs(input.A[1]);  
	float v = 0;
	for (int d = 0; d < DIMENSION ; d++)
	{
		v += fabs(input[d]);
	}
	return v;
}
float TestFunctions::DeJong4(float* input)
{
	//return 1 * input.A[0] * input.A[0] * input.A[0] * input.A[0]
		//+ 2 * input.A[1] * input.A[1] * input.A[1] * input.A[1];
	float v = 0;
	for (int d = 0; d < DIMENSION; d++)
	{
		v += (d + 1)*input[d] * input[d] * input[d] * input[d];
	}
	return v;
}
float TestFunctions::Rastrigin(float* input)
{
	//return input.A[0] * input.A[0] - 10 * cosf(input.A[0])
		//+ input.A[1] * input.A[1] - 10 * cosf(input.A[1]); 
	float v = 0;
	for (int d = 0; d < DIMENSION; d++)
	{
		v += input[d] * input[d] - 10 * cosf(2 * input[d]);
	}
	return v*2*10;
}
float TestFunctions::Griewangk(float* input)
{
	//return 1 + input.A[0] * input.A[0] / 4000 + input.A[1] * input.A[1] / 4000
		//+ cos(input.A[0] / sqrtf(input.A[0]))*cos(input.A[1] / sqrtf(input.A[1]));
	float v = 0;
	for (int d = 0; d < DIMENSION; d++)
	{
		v+=input[d] * input[d] / 4000;
	}
	float v2 = 1;
	for (int d = 0; d < DIMENSION; d++)
	{
		v2 *= cosf(input[d] / sqrtf(input[d]));
	}
	v -= v2;
	return v+1;
}
float TestFunctions::StretchedV(float* input)
{
	//return pow((input.A[0] * input.A[0] + input.A[1] * input.A[1]), 4)
		//* (sinf(50*(powf(input.A[0] * input.A[0] + input.A[1] * input.A[1],10))) + 1);
	float v = 0;
	for (int d = 0; d < DIMENSION - 1; d++)
	{
		v -= 0.5f + sin(sqrtf(input[d] * input[d] + input[d + 1] * input[d + 1]) - 0.5f)*sin(sqrtf(input[d] * input[d] + input[d+1] * input[d+1])-0.5f) 
			/ ((1+0.0001*(input[d] * input[d]+ input[d + 1] * input[d + 1]))*(1 + 0.0001*(input[d] * input[d] + input[d + 1] * input[d + 1])));
	}
	return v;
}
float TestFunctions::Ackley(float* input)
{
	//return 0.01*sqrtf(input.A[0] * input.A[0] + input.A[1] * input.A[1])
		//+ 3 * (cosf(2 * input.A[0]) + sinf(2 * input.A[1]));
	float v = 0; 
	for (int d = 0; d < DIMENSION - 1; d++)
	{
		v += 0.81873075f*sqrtf(input[d] * input[d] + input[d + 1] * input[d + 1])
			+ 3 * (cosf(2 * input[d]) + sin(2 * input[d + 1]));
	}
	return v;
}
float TestFunctions::EggHolder(float* input)
{
	/*return (-input.A[0] * sinf(sqrt(fabsf(input.A[0] - (input.A[1] + 47)))))
		- (input.A[1] + 47)*sinf(sqrtf(input.A[1] + 47 + input.A[0]/2));*/
	float v = 0;
	for (int d = 0; d < DIMENSION - 1; d++)
	{
		v -= input[d] * sinf(sqrt(fabsf(input[d] - (input[d + 1] - 47))))
			- (input[d + 1] + 47)*sinf(sqrtf(fabsf(input[d+1]+47+input[d]/2)));
	}
	return v;
}
float TestFunctions::Rana(float* input)
{
	/*float a = sqrtf(fabsf(input.A[1]-1- input.A[0]));
	float b = sqrtf(fabsf(input.A[1] - 1 + input.A[0]));
	return input.A[0] * sinf(a)*cosf(b) + (input.A[1] + 1)*sinf(b)*cosf(a);*/
	float v = 0;
	for (int d = 0; d < DIMENSION - 1; d++)
	{
		float a = sqrtf(fabs(input[d + 1] + 1 - input[d]));
		float b = sqrtf(fabs(input[d + 1] + 1 + input[d]));
		v += input[d] * sinf(a)*cosf(b) + (input[d + 1] + 1)*sinf(b)*cosf(a);
	}
	return v;
}
float TestFunctions::Pathological(float* i)
{
	//return 0.5f + (sinf(sqrtf(100* input.A[0] * input.A[0] + input.A[1] * input.A[1])*sinf(sqrtf(100 * input.A[0] * input.A[0] + input.A[1] * input.A[1]) -0.5)) 
		/// (1 + 0.001*(input.A[0]* input.A[0]-2* input.A[0] * input.A[1] + input.A[1]* input.A[1])));
	float  v = 0;
	for (int d = 0; d < DIMENSION - 1; d++)
	{
		v += 0.5f + sinf(sqrtf(100 * i[d] * i[d] + i[d + 1] * i[d + 1]) - 0.5f)
			/ (1 + 0.001f*(i[d] * i[d] - 2 * i[d] * i[d + 1] + i[d+1]* i[d + 1]));
	}
	return v;
}
float TestFunctions::Michalewicz(float* i)
{
	//return -1 * sinf(input.A[0])*powf(sinf(input.A[0] * input.A[0] / 3.14), 20)
		//+ sinf(input.A[1])*pow(2 * input.A[1] * input.A[1] / 3.14, 20);
	float v = 0;
	for (int d = 0; d < DIMENSION; d++)
	{
		v -= sinf(i[d] * powf(sinf(i[d] * i[d] / 3.14f), 20))
			+ sinf(i[d+1] * powf(sinf(i[d+1] * i[d+1] / 3.14f), 20));
	}
	return v;
}
float TestFunctions::Masters_InvertedCosine(float* i)
{
	//return powf(10,-(input.A[0]* input.A[0]  + input.A[1]* input.A[1] + 0.5*input.A[0]* input.A[1])/8 )
		//* cosf(4*sqrtf(input.A[0] * input.A[0] + input.A[1] * input.A[1] + 0.5*input.A[0] * input.A[1]));
	float v = 0;
	for (int d = 0; d < DIMENSION-1; d++)
	{
		v -= powf(2.71f, (-(i[d] * i[d] + i[d + 1] * i[d + 1] + 0.5*i[d] * i[d + 1])) / 8)
			*cosf(4*sqrtf(i[d]*i[d] + i[d+1] * i[d+1] + 0.5f*i[d]*i[d+1]));
	}
	return v;
}

float TestFunctions::DeJong(TVector input)
{
	return input.A[0] * input.A[0] + input.A[1] * input.A[1];
}
float TestFunctions::Rosenbrock(TVector input)
{
	return 100 * (input.A[0] * input.A[0] - input.A[1] * input.A[1]) + (1 - input.A[0] * input.A[0])*(1 - input.A[0] * input.A[0]);
}
float TestFunctions::DeJong3(TVector input)
{
	return fabs(input.A[0]) + fabs(input.A[1]);
}
float TestFunctions::DeJong4(TVector input)
{
	return 1 * input.A[0] * input.A[0] * input.A[0] * input.A[0]
		+ 2 * input.A[1] * input.A[1] * input.A[1] * input.A[1];
}
float TestFunctions::Rastrigin(TVector input)
{
	return input.A[0] * input.A[0] - 10 * cosf(input.A[0])
		+ input.A[1] * input.A[1] - 10 * cosf(input.A[1]);
}
float TestFunctions::Griewangk(TVector input)
{
	return 1 + input.A[0] * input.A[0] / 4000 + input.A[1] * input.A[1] / 4000
		+ cos(input.A[0] / sqrtf(input.A[0]))*cos(input.A[1] / sqrtf(input.A[1]));
}
float TestFunctions::StretchedV(TVector input)
{
	return pow((input.A[0] * input.A[0] + input.A[1] * input.A[1]), 4)
		* (sinf(50 * (powf(input.A[0] * input.A[0] + input.A[1] * input.A[1], 10))) + 1);
}
float TestFunctions::Ackley(TVector input)
{
	return 0.01*sqrtf(input.A[0] * input.A[0] + input.A[1] * input.A[1])
		+ 3 * (cosf(2 * input.A[0]) + sinf(2 * input.A[1]));
}
float TestFunctions::EggHolder(TVector input)
{
	return (-input.A[0] * sinf(sqrt(fabsf(input.A[0] - (input.A[1] + 47)))))
		- (input.A[1] + 47)*sinf(sqrtf(input.A[1] + 47 + input.A[0] / 2));
}
float TestFunctions::Rana(TVector input)
{
	float a = sqrtf(fabsf(input.A[1] - 1 - input.A[0]));
	float b = sqrtf(fabsf(input.A[1] - 1 + input.A[0]));
	return input.A[0] * sinf(a)*cosf(b) + (input.A[1] + 1)*sinf(b)*cosf(a);
}
float TestFunctions::Pathological(TVector input)
{
	return 0.5f + (sinf(sqrtf(100 * input.A[0] * input.A[0] + input.A[1] * input.A[1])*sinf(sqrtf(100 * input.A[0] * input.A[0] + input.A[1] * input.A[1]) - 0.5))
		/ (1 + 0.001*(input.A[0] * input.A[0] - 2 * input.A[0] * input.A[1] + input.A[1] * input.A[1])));
}
float TestFunctions::Michalewicz(TVector input)
{
	return -1 * sinf(input.A[0])*powf(sinf(input.A[0] * input.A[0] / 3.14), 20)
		+ sinf(input.A[1])*pow(2 * input.A[1] * input.A[1] / 3.14, 20);
}
float TestFunctions::Masters_InvertedCosine(TVector input)
{
	return powf(10, -(input.A[0] * input.A[0] + input.A[1] * input.A[1] + 0.5*input.A[0] * input.A[1]) / 8)
		* cosf(4 * sqrtf(input.A[0] * input.A[0] + input.A[1] * input.A[1] + 0.5*input.A[0] * input.A[1]));
}

//
void TestFunctions::SetTestfunction2(int index)
{
	switch (index)
	{
	case 0: testfunction_ = &DeJong; break;
	case 1: testfunction_ = &Schwefel_; break;
	case 2: testfunction_ = &Rosenbrock; break;
	case 3: testfunction_ = &DeJong3; break;
	case 4: testfunction_ = &DeJong4; break;
	case 5: testfunction_ = &Rastrigin; break;
	case 6: testfunction_ = &Griewangk; break;
	case 7: testfunction_ = &StretchedV; break;
	case 8: testfunction_ = &Ackley; break;
	case 9: testfunction_ = &EggHolder; break;
	case 10: testfunction_ = &Rana; break;
	case 11: testfunction_ = &Pathological; break;
	case 12: testfunction_ = &Michalewicz; break;
	case 13: testfunction_ = &Masters_InvertedCosine; break;
	default:
		testfunction = &Schwefel;
		break;
	}

}

char* TestFunctions::GetTestFunctionName()
{
	switch (TEST_FUNCTION_ID)
	{
	case 0: return "DeJong";
	case 1: return "Schwefel";
	case 2: return "Rosenbrock";
	case 3: return "DeJong3";
	case 4: return "DeJong4";
	case 5: return "Rastrigin";
	case 6: return "Griewangk";
	case 7: return "StretchedV";
	case 8: return "Ackley";
	case 9: return "EggHolder";
	case 10: return "Rana";
	case 11: return "Pathological";
	case 12: return "Michalewicz";
	case 13: return "Masters_InvertedCosine";
	default: return "noname";
	}

}