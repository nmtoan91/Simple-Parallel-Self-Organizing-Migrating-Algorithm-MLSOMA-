#include "MSOMA.h"

MSOMA::MSOMA()
{
	
}
MSOMA::MSOMA(int n)
{
	this->n = n;
	SOMAs = (SOMA_Sequence**)malloc(n * sizeof(SOMA_Sequence*));
	for (int i = 0; i < n; i++)
	{
		SOMAs[i] = new SOMA_Sequence();
	}
}
MSOMA::~MSOMA()
{
}
