#pragma once
#include <fstream>
using namespace std;

class TOutput
{
public:
	ofstream* ofs;
public:
	TOutput();
	~TOutput();
	void WriteMigration(float* Individuals, float *Individuals_cost, int LEADER_INDEX);
	static TOutput& getInstance() { static TOutput    instance; return instance; }
};

