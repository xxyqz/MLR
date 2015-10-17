// regress.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include"regress.h"

int main()
{
	
	matrix.read_data();
	matrix.fold = 5;
	//matrix.cvleaveoneout(matrix.x, matrix.y);
	matrix.cvkfold(matrix.x, matrix.y, matrix.fold);
	matrix.r2(matrix.x, matrix.y);
	
	


	return 0;
}

