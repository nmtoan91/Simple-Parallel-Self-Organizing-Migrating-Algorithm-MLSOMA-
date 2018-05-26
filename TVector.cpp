#include "TVector.h"


TVector::TVector(void)
{
	A[0] = A[0] =0;
	LENGTH = 2;
}


TVector::~TVector(void)
{
}
TVector::TVector(int length)
{
	LENGTH = length;
}
TVector::TVector(float a0,float a1)
{
	LENGTH = 2;
	A[0] = a0;
	A[1] = a1;
}

TVector operator +(const TVector& s1,const TVector& s2)
{
	TVector c;
	for (int i = 0; i<s1.LENGTH; i++)
	{
		c.A[i] = s1.A[i]+s2.A[i];
	}
	c.LENGTH = s1.LENGTH;
	return c;
}
TVector operator *(const TVector& s1,const TVector& s2)
{
	TVector c;
	for (int i = 0; i<s1.LENGTH; i++)
	{
		c.A[i] = s1.A[i]*s2.A[i];
	}
	c.LENGTH = s1.LENGTH;
	return c;
}
TVector operator *(const TVector& s1,const float& s2)
{
	TVector c;
	for (int i = 0; i<s1.LENGTH; i++)
	{
		c.A[i] = s1.A[i]*s2;
	}
	c.LENGTH = s1.LENGTH;
	return c;
}
TVector operator *(const float& s1,const TVector& s2)
{
	TVector c;
	for (int i = 0; i<s2.LENGTH; i++)
	{
		c.A[i] = s2.A[i]*s1;
	}
	c.LENGTH = s2.LENGTH;
	return c;
}
TVector operator -(const TVector& s1,const TVector& s2)
{
	TVector c;
	for (int i = 0; i<s1.LENGTH; i++)
	{
		c.A[i] = s1.A[i]-s2.A[i];
	}
	c.LENGTH = s1.LENGTH;
	return c;
}
