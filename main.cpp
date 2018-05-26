//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"
#include <stdio.h>
#include "TestFunctions.h"
#include <stdio.h>
#include "SOMA_Sequence.h" 
#include "MLSOMA.h"
#include "MLSOMA2.h"
#include "MPITool.h" 
#include "TimeAnalysis.h"   
 
int main(int argc, char *argv[])
{
	InitParameters(argc, argv);
	if (METHOD == 0)  //SOMA single node
	{
		SOMA_Sequence soma;
		soma.SetBounds(-RANGE, RANGE);
		soma.RanDomIndividuals();
		
			soma.DoSOMAWithSchwefel();
	}
	else if (METHOD == 1) //SOMA multiple nodes, use 
	{
		MPITool::getInstance().Init(argc, argv);
		MLSOMA mlsoma;
		mlsoma.SetBoundsGlobal(-RANGE, RANGE);
		mlsoma.Run();
		MPITool::getInstance().MPI_CallFinalize();
	}
	else if (METHOD == 2) //SOMA multiple node use multople group
	{
		MPITool::getInstance().Init(argc, argv);
		MLSOMA2 mlsoma2;
		mlsoma2.SetBounds(-RANGE, RANGE);
		mlsoma2.Run();
		MPITool::getInstance().MPI_CallFinalize();
	}
	if ((WORLD_SIZE > 1 && PROCESS_ID == 0) || WORLD_SIZE == 1)
		TimeAnalysis::getInstance().WriteResult(TimeAnalysis::getInstance().BuildFilename());
	return 0;
}

