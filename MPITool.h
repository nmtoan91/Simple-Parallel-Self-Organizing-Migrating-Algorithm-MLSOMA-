#pragma once
#include "TDefine.h"
#include "mpi.h"
class MPITool
{
public:
	MPITool();
	~MPITool();
	static MPITool& getInstance() { static MPITool    instance; return instance; } 
	void Init(int argc, char *argv[]);
	void MPI_CallFinalize();
};

