#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class VRP {

public:

	int VRPMaster();
	int VRPSolver(int noderBesoktArray[antallNoder][t], int t, int antallNoder);
	int solveVRPFunction(vector<int>& rekkefolge, vector<int>& v, vector<int>& oversikt);
	int getcapacity();																		//Må lages
	int getMaxTravel();																		//Må lages
	void noderBesoktArrayFiller(int* noderBesoktArray);

private:

	int solution = 0;

};



