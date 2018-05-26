#pragma once
#include "SOMA_Sequence.h"
class SOMAMPIMessage
{
private:
	SOMA_Sequence *SOMA;
	
public:
	int size;
	float* ARRAY;
public:
	SOMAMPIMessage();
	SOMAMPIMessage(SOMA_Sequence *soma);
	~SOMAMPIMessage();
};

