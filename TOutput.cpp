#include "TOutput.h"
#include "TDefine.h"

TOutput::TOutput()
{
	char buf[256];
	sprintf(buf, "migration_core%d.csv", MY_DEVICE_ID);
	if(PROCESS_ID==0)
		ofs = new ofstream(MIGRATION_NAME, std::ofstream::out);
	else ofs = new ofstream(buf, std::ofstream::out);
}


TOutput::~TOutput()
{
	ofs->close();
}
void TOutput::WriteMigration(float* Individuals, float *Individuals_cost, int LEADER_INDEX)
{
	*ofs << LEADER_INDEX << ",";
	for (int i = 0; i < POP_SIZE; i++)
	{
		for (int d = 0; d < DIMENSION; d++)
		{
			*ofs << Individuals[i*DIMENSION+d] << ",";
		}
		*ofs << Individuals_cost[i];
		if(i <POP_SIZE-1) *ofs << ",";
		else *ofs << endl;
	}
}