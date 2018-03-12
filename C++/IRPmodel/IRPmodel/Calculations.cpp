#include "stdafx.h"
#include "Calculations.h"

using namespace DataSets;


Calculations::Calculations(vector<double> *** modelData, int instances, int periods)
	:
	nInstances(instances),
	nPeriods(periods),
	nModels(ModelList.size()),
	nFields(ContentList.size())
{

	//Initialize dataholders
	total = new double**[nModels];
	totalAvg = new vector<double> *[nModels];
	varTot = new double **[nModels];
	var = new double **[nModels];

	for (auto i : ModelList) {
		total[i] = new double *[nFields];
		totalAvg[i] = new vector<double> [nFields];
		varTot[i] = new double *[nModels];
		var[i] = new double *[nModels];

		for (auto j : ContentList) {
			total[i][j] = new double[nPeriods + 1];
			totalAvg[i][j].resize(nPeriods + 1);
			varTot[i][j] = new double[nPeriods + 1];
			var[i][j] = new double[nPeriods + 1];


			for (int t = 1; t <= nPeriods; t++) {
				total[i][j][t] = 0;
				totalAvg[i][j][t] = 0;
				varTot[i][j][t] = 0;
				var[i][j][t] = 0;
			}
		}
	}

	AnalyzeModel(modelData);
}

int Calculations::AnalyzeModel(vector<double> *** modelData)
{

	//Calculate the average over all instances
	for (auto model : ModelList)
		for (auto field : ContentList)
			for (int period = 1; period <= nPeriods;)
				totalAvg[model][field][period] += getAverage(modelData[model][field][period]);


	//Calculate the sample standard deviation over all instances
	for (auto model : ModelList)
		for (auto field : ContentList)
			for (int period = 1; period <= nPeriods; period++)
				var[model][field][period] += getStdDev(modelData[model][field][period], totalAvg[model][field][period]);
}

double Calculations::getAverage(vector<double> values)
{
	double avg = 0;
	for (auto v : values)
		avg += v;

	return avg;
}
double Calculations::getStdDev(vector<double> v, double average = -1)
{
	double stdDev = 0;
	if (average == -1)
		average = getAverage(v);

	for (auto val : v)
		stdDev = sqrt(pow(val - average, 2) / (v.size() - 1));

	return stdDev;
}



double  ** Calculations::getCostDistribution()
{
	vector <int> CostSet = { HoldingCost, TransCost, Objective };

	double *** CostData;
	vector<double> * hold;
	vector <double> * trans;
	vector <double> * deltaH;
	vector<double> * deltaT;
	vector<double> * deltaObj;

	hold = new vector<double>[nInstances];
	trans = new vector<double>[nInstances];
	deltaH = new vector<double>[nInstances];
	deltaT = new vector<double>[nInstances];
	deltaObj = new vector<double>[nInstances];

	for (int t = 1; t <= nPeriods; t++) {
		hold[t].resize(nInstances);
		trans[t].resize(nInstances);
		deltaH[t].resize(nInstances);
		deltaT[t].resize(nInstances);
		deltaObj[t].resize(nInstances);
	}
	//Calculate the average cost over all periods
	double ** totCostAvg;
	totCostAvg = new double*[ModelList.size()];

	for (auto model : ModelList) {
		totCostAvg[model] = new double[CostSet.size()];
			for (auto cost : CostSet) {
				{
					totCostAvg[model][cost] = getAverage(totalAvg[model][cost]);
				}
			}
	}
	//Calculate cost distribution for each period
	for (int t = 1; t <= nPeriods; t++) {
		hold[t] = totalAvg[Exact][HoldingCost][t] / totalAvg[Exact][Objective][t] * 100;
		trans[t] = totalAvg[Exact][TransCost][t] / totalAvg[Exact][Objective][t] * 100;

	
		deltaH[t] = (totalAvg[Construction][HoldingCost][t] - totalAvg[Exact][HoldingCost][t]) / totalAvg[Exact][HoldingCost][t] * 100;
		deltaT[t] = (totalAvg[Construction][HoldingCost][t] - totalAvg[Exact][TransCost][t]) / totalAvg[Exact][TransCost][t] * 100;
		deltaObj[t] = (totalAvg[Construction][Objective][t] - totalAvg[Exact][Objective][t]) / totalAvg[Exact][Objective][t] * 100;


		CostData[HoldExact][t][Average] = hold[t];
		CostData[HoldeExact][t][StdDev] = getStdDev[hold];
		CostData[TransExact][t] = trans[t];
		CostData[deltaHold][t] = deltaH[t];
		CostData[deltaTrans][t] = deltaT[t];
		CostData[deltaObjective][t] = deltaObj[t];
	}

	//Calculate the cumulative cost distribution
	double holdTot = totCostAvg[Exact][HoldingCost]/ totCostAvg[Exact][Objective] * 100;
	double transTot = totCostAvg[Exact][TransCost] / totCostAvg[Exact][Objective] * 100;

	double deltaHTot = (totCostAvg[Construction][HoldingCost] - totCostAvg[Exact][HoldingCost]) / totCostAvg[Exact][HoldingCost] * 100;
	double deltaTTot = (totCostAvg[Construction][HoldingCost] - totCostAvg[Exact][TransCost]) / totCostAvg[Exact][TransCost] * 100;
	double deltaObjTot = (totCostAvg[Construction][Objective] - totCostAvg[Exact][Objective]) / totCostAvg[Exact][Objective] * 100;

	CostData[HoldExact][nPeriods + 1] = holdTot;
	CostData[TransExact][nPeriods + 1] = transTot;
	CostData[deltaHold][nPeriods + 1] = deltaHTot;
	CostData[deltaTrans][nPeriods + 1] = deltaTTot;
	CostData[deltaObjective][nPeriods + 1] = deltaObjTot;

	return CostData;
}
				




Calculations::~Calculations()
{
}
