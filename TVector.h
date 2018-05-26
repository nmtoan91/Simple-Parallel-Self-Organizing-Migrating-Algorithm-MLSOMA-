#pragma once
//#define TVECTOR_MAX_LENGTH 2
class TVector
{
public:
	float A[100];
	int LENGTH; 
public:
	TVector(void);
	TVector(int length);
	~TVector(void);
	TVector(float a0,float a1);
	friend TVector operator +(const TVector& s1,const TVector& s2);
	friend TVector operator *(const TVector& s1,const TVector& s2);
	friend TVector operator *(const TVector& s1,const float& s2);
	friend TVector operator *(const float& s1,const TVector& s2);
	friend TVector operator -(const TVector& s1,const TVector& s2);
	//friend TVector operator /(const TVector& s1,const TVector& s2);
};

