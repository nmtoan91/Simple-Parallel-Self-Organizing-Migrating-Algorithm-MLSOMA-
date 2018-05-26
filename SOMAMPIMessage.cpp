#include "SOMAMPIMessage.h"
#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <iostream>
SOMAMPIMessage::SOMAMPIMessage()
{
}
SOMAMPIMessage::~SOMAMPIMessage()
{
}
SOMAMPIMessage::SOMAMPIMessage(SOMA_Sequence *soma)
{
	this->SOMA = soma;
	int count = 0; 
	for (int i = 0; i < POP_SIZE; i++)
	{
		if (SOMA->Individuals_isChanged[i]) count++;
	}
	//should use boolean
	size = count + count*DIMENSION;
	ARRAY = (float*)malloc(size*sizeof(float)); 
	int count2 =0 ;
	for (int i = 0; i < POP_SIZE; i++)
	{
		if (SOMA->Individuals_isChanged[i])
		{
			ARRAY[i] = 1;
			int index = count + count2*DIMENSION;
			std::memcpy(&ARRAY[index], &SOMA->Individuals[DIMENSION*i], DIMENSION);
			count2++;
		}
		else ARRAY[i] = 0;

	}
		
}
