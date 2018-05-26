#pragma once
#include "SOMA_Sequence.h"
#include <stdlib.h>
class MSOMA
{
public:
	SOMA_Sequence** SOMAs;
	int n;
public:
	MSOMA();
	MSOMA(int n);
	~MSOMA();
};

