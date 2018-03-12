#pragma once

#include <stdlib.h>
#include <initializer_list>
#include "DataSets.h"
#include <vector>

using namespace std;

class Calculations
{
public:
	Calculations(vector <double> *** modelData, int instances, int periods);
	double ** getCostDistribution();
	//Returns cost data for each time period and total
	~Calculations();
private:
	int AnalyzeModel(vector <double> *** modelData);
	double getStdDev(vector <double> values, double average);
	double getAverage(vector <double> values);


	int nInstances;
	int nPeriods;
	int nModels;
	int nFields;

	double *** total;
	vector<double> ** totalAvg;
	double *** varTot;
	double *** var;
};

