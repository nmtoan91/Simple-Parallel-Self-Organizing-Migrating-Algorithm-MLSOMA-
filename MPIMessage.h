#pragma once
class MPIMessage
{
public:
	float *BUFFER;  
	int LEADER_INDEX;  
	//int POP_SIZE;   
public:
	MPIMessage();
	~MPIMessage();
};

