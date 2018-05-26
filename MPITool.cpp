#include "MPITool.h"
#include "TDefine.h"


MPITool::MPITool()
{
}


MPITool::~MPITool()
{
}
void MPITool::Init(int argc, char *argv[]) 
{
	int provided;
#ifdef __linux__
	MPI_Init(&argc, &argv); 
#else
	MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
	if (provided != MPI_THREAD_MULTIPLE)
	{
		printf((char*)"Sorry, this MPI implementation does not support multiple threads\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
#endif

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	WORLD_SIZE = world_size;
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	PROCESS_ID = world_rank;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	printf((char*)"Processor %s, rank %d out of %d processors\n",processor_name, PROCESS_ID, WORLD_SIZE);


	if (WORLD_SIZE == 1) MY_DEVICE_ID = 0; 
	else MY_DEVICE_ID = PROCESS_ID - 1;
}
void MPITool::MPI_CallFinalize()
{
	MPI_Finalize(); 
}