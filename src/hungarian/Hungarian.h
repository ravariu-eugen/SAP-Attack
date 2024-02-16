///////////////////////////////////////////////////////////////////////////////
// Hungarian.h: Header file for Class HungarianAlgorithm.
// 
// This is a C++ wrapper with slight modification of a hungarian algorithm implementation by Markus Buehren.
// The original implementation is a few mex-functions for use in MATLAB, found here:
// http://www.mathworks.com/matlabcentral/fileexchange/6543-functions-for-the-rectangular-assignment-problem
// 
// Both this code and the orignal code are published under the BSD license.
// by Cong Ma, 2016
// 

#ifndef HUNGARIAN_H
#define HUNGARIAN_H

#include <iostream>
#include <vector>

using namespace std;


class HungarianAlgorithm
{
public:
	HungarianAlgorithm();
	~HungarianAlgorithm();
	double Solve(vector <vector<double> >& DistMatrix, vector<int>& Assignment);

private:
	int *assignment; // 
	double *distMatrix; 
	bool *starMatrix, *newStarMatrix, *primeMatrix; 
	bool *coveredColumns, *coveredRows; 
	int nOfRows, nOfColumns, minDim;
	int row, col;


	double assignmentoptimal(double *distMatrix);
	void buildassignmentvector();
	double computeassignmentcost(double *distMatrix);
	int step2a();
	int step2b();
	int step3();
	int step4();
	int step5();
};


#endif